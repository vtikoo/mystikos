#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int rand_range(int min, int max)
{
    int range = max - min + 1;
    // when min > 0:
    // For eg: [1 2 3 4 5]
    //         min     max
    // range = 5
    // this is also equivalent to: [0 1 2 3 4]
    // shifted_max = 5 - 1
    //
    // When min == 0:
    // For eg: [0 1 2 3 4 5]
    //         min        max
    // algo: rand() % 6
    max = max - min;

    int rand_int = rand() % (max + 1);
    // rand() returns num b/w 0 and RAND_MAX
    // % max + 1 scales that to 0 to max;

    // shift right for min > 0
    return rand_int + min;
}

#define PAGE_SIZE 4096

typedef enum
{
    CHMEMPROT,
    UNMAP,
    EXIT
} memops_t;

typedef enum
{
    NONE,
    R,
    W,
    RW,
    RWX,
    RX
} prot_t;

char* prot_to_string(prot_t prot)
{
    switch (prot)
    {
        case NONE:
            return "n";
        case R:
            return "r";
        case RW:
            return "rw";
        case RWX:
            return "rwx";
        case W:
            return "w";
        case RX:
            return "rx";
        default:
            return "unknown";
    }
}
prot_t get_rand_prot()
{
    return rand_range(0, 5);
}

prot_t get_rand_memop()
{
    return rand_range(0, 2);
}

typedef struct node
{
    struct node* next;
    struct node* prev;
} node_t;

typedef struct
{
    node_t* head;
    node_t* tail;
} list_t;

typedef struct
{
    int pid;
    list_t maps; // list_t<span_t>
} proc_t;

typedef struct
{
    void* addr;
    int length;
    int prot;
} mem_range_t;

struct mem_range_node
{
    node_t node;
    mem_range_t range;
};

typedef struct mem_range_node span_t;

typedef struct
{
    node_t node;
    mem_range_t range;
    list_t spans;
} shrd_region_t;

typedef struct
{
    node_t node;
    mem_range_t range;
    shrd_region_t* shr;
} mem_map_t;

// list_t shrd_region_t; // list_t<shrd_region_t>
// list_t procs; // list_t<proc_t>
shrd_region_t* shared;
proc_t* pid1;

void* add_span(list_t* list, span_t* span)
{
    if (!list->head && !list->tail)
    {
        list->head = (node_t*)span;
        list->tail = (node_t*)span;
    }
}

proc_t* pid_to_proc(int pid)
{
    return pid1;
}

int notify_shared_region(
    shrd_region_t* shr,
    mem_range_t* range,
    int oldprot,
    int newprot)
{
}

int init(int nprocs, int npages)
{
    shared = calloc(1, sizeof(shrd_region_t));
    shared->range.length = npages * PAGE_SIZE;
    shared->range.prot = get_rand_prot();

    printf(
        "addr=%p length=%d prot=%s\n",
        shared->range.addr,
        shared->range.length,
        prot_to_string(shared->range.prot));

    static int pid = 1;
    pid1 = calloc(1, sizeof(proc_t));
    pid1->pid = pid;

    {
        mem_map_t* map = calloc(1, sizeof(mem_map_t));
        map->range.addr = shared->range.addr;
        map->range.length = shared->range.length;
        map->range.prot = shared->range.prot;
        map->shr = shared;

        add_span(&pid1->maps, map);
    }
}

#define ROUNDUP(x, n) ((x + n - 1) & ~(n - 1))

int chmemprot(int pid, char* addr, int length, int prot)
{
    proc_t* proc = pid_to_proc(pid);
    mem_map_t* map = (mem_map_t*)proc->maps.head;
    while (map)
    {
        if (map->range.addr <= addr && addr <= addr + length)
        {
            // prot unchanged, nothing to do
            if (map->range.prot == prot || length == 0)
                return 0;

            // exact match
            if (map->range.addr == addr &&
                ROUNDUP(map->range.length, PAGE_SIZE) ==
                    ROUNDUP(length, PAGE_SIZE))
                map->range.prot == prot;
            // leading
            // trailing
            // hole
        }
        map = map->node.next;
    }
    return -1;
}

int unmap(int pid, char* addr, int length)
{
}

int main()
{
    srand(time(0));

    int npages = 10;
    int nprocs = 2;

    init(nprocs, npages);
}