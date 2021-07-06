// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <spawn.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <unistd.h>

#define CORERUN_PATH "/coreclr-tests-all/Tests/Core_Root/corerun"

static int _run_tests(const char* test_file)
{
    FILE* file = fopen(test_file, "r");

    if (!file)
    {
        fprintf(stderr, "File %s not found \n", test_file);
        return -1;
    }
    char line[256];
    int i = 1;

    posix_spawn_file_actions_t action;
    posix_spawn_file_actions_init(&action);
    posix_spawn_file_actions_addopen(
        &action, STDOUT_FILENO, "/dev/null", O_WRONLY | O_APPEND, 0);
    posix_spawn_file_actions_addopen(
        &action, STDERR_FILENO, "/dev/null", O_WRONLY | O_APPEND, 0);
    struct sysinfo info;
    sysinfo(&info);
    long prev_freeram = info.freeram;
    printf("freeram at startup = %ld\n", info.freeram);
    while (fgets(line, sizeof(line), file))
    {
        line[strlen(line) - 1] = '\0';
        int r;
        pid_t pid;
        int wstatus;

        printf("=== start test %d: %s\n", i++, line);

        char* const args[] = {CORERUN_PATH, (char*)line, NULL};
        char* const envp[] = {"COMPlus_EnableDiagnostics=0",
                              "COMPlus_EnableAlternateStackCheck=1",
                              NULL};

        r = posix_spawn(&pid, CORERUN_PATH, &action, NULL, args, envp);

        assert(r == 0);
        assert(pid >= 0);

        assert(waitpid(pid, &wstatus, 0) == pid);
        assert(WIFEXITED(wstatus));
        // exit code 100 represents success in coreclr tests?!
        if ((r = WEXITSTATUS(wstatus)) != 100)
        {
            printf("!!! WEXITSTATUS(wstatus) = %d\n", r);
            assert(0);
        }
        printf("=== passed test (%s)\n", line);
        struct sysinfo info;
        sysinfo(&info);
        printf(
            "freeram= %ld leak=%ld\n",
            info.freeram,
            prev_freeram - info.freeram);
        prev_freeram = info.freeram;
    }

    fclose(file);
    return 0;
}

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Must pass in the file containing test names\n");
        return -1;
    }

    int ret = _run_tests(argv[1]);
    if (ret == 0)
        printf("=== passed all tests: %s\n", argv[0]);
    return ret;
}