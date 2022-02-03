#include <assert.h>
#include <fcntl.h>
#include <printf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_SIZE 10

int main(int argc, char* argv[])
{
    if (strcmp(argv[1], "first") == 0)
    {
        int fd = shm_open("/shm-fun-1", O_RDWR, 0);
        assert(fd >= 0);

        char* addr =
            mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        assert(addr != MAP_FAILED);

        printf("addr=%p contents=%s\n", addr, addr);

        char c;
        while ((c = getchar()) != 'c')
        {
            sleep(1);
            printf("addr: %s\n", addr);
        }
    }
    else if (strcmp(argv[1], "second") == 0)
    {
        int fd = shm_open("/shm-fun-1", O_RDWR, 0);
        assert(fd >= 0);

        char* addr =
            mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        assert(addr != MAP_FAILED);

        printf("addr=%p contents=%s\n", addr, addr);

        char c;
        while ((c = getchar()) != 'c')
        {
            sleep(1);
        }

        strcpy(addr, "hellzwrld");
        while ((c = getchar()) != 'c')
        {
            sleep(1);
        }
    }
}
