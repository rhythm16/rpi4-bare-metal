#include "mm.h"
#include "types.h"
#include "sched.h"

u64 KERNEL_PA_BASE;

static u8 mem_map[MALLOC_PAGES] = {0, };

/* returns a kva of a free page */
u64 get_kernel_page()
{
    u64 phys_page = get_free_page();
    if (phys_page == 0)
        return 0;
    return PA_TO_KVA(phys_page);
}

/* returns a pa of a free page */
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

