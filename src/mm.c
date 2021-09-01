#include "mm.h"
#include "types.h"

u64 KERNEL_PA_BASE;

static u8 mem_map[MALLOC_PAGES] = {0, };

u64 get_free_page()
{
    for (int i = 0; i < MALLOC_PAGES; i++) {
        if (mem_map[i] == 0) {
            mem_map[i] = 1;
            return MALLOC_START + i * PAGE_SIZE;
        }
    }
    return 0;
}

void free_page(u64 p)
{
    mem_map[(p - MALLOC_START) / PAGE_SIZE] = 0;
}
