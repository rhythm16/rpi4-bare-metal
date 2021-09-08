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

/* bytes is round down to the nearest 8 */
void memcpy(u64* dst, u64* src, u64 bytes)
{
    u64 num = bytes >> 3;
    for (u64 i = 0; i < num; i++) {
        dst[i] = src[i];
    }
}

