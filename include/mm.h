#ifndef MM_H
#define MM_H

#define PAGE_SHIFT 12
#define TABLE_SHIFT 9
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)
#define PAGE_SIZE (1 << PAGE_SHIFT)
#define SECTION_SIZE (1 << SECTION_SHIFT)

#define LOW_MEMORY (2* SECTION_SIZE)

#define MALLOC_START 0x40000000
#define MALLOC_END   0xFC000000
#define MALLOC_SIZE  (MALLOC_END - MALLOC_START)
#define MALLOC_PAGES (MALLOC_SIZE / PAGE_SIZE)
#define THREAD_SIZE  PAGE_SIZE

#ifndef __ASSEMBLER__

#include "types.h"

void memzero(u64 src, unsigned int n);
u64 get_free_page();
void free_page(u64 p);

extern u64 _start;
#define KERNEL_START (&(_start))

#endif /* __ASSEMBLER__ */

#endif /* MM_H */
