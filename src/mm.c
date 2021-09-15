#include "mm.h"
#include "types.h"
#include "sched.h"
#include "utils.h"
#include "mmu.h"

u64 KERNEL_PA_BASE;

static u8 mem_map[MALLOC_PAGES] = {0, };

/* returns the kva of a page that is mapped to a user process at va */
u64 allocate_user_page(struct task_struct *task, u64 uva)
{
    u64 phys_page = get_free_page();
    if (map_page(task, uva, phys_page) < 0)
        return 0;
    return PA_TO_KVA(phys_page);
}

/* returns 0: success, <0: error */
int map_page(struct task_struct *task, u64 uva, u64 phys_page)
{
    if (!task->mm.pgd) {
        int kp_count = task_kp_count(task);
        if (kp_count == MAX_PAGE_COUNT)
            return -1;
        task->mm.pgd = get_free_page();
        task->mm.kernel_pages[kp_count] = task->mm.pgd;
    }
    u64 pgd = task->mm.pgd;
    int new_table;

    u64 pud = map_table((u64*)PA_TO_KVA(pgd), PGD_SHIFT, uva, &new_table);
    if (new_table) {
        int kp_count = task_kp_count(task);
        if (kp_count == MAX_PAGE_COUNT) {
            free_page(pud);
            return -1;
        }
        task->mm.kernel_pages[kp_count] = pud;
    }

    u64 pmd = map_table((u64*)PA_TO_KVA(pud), PUD_SHIFT, uva, &new_table);
    if (new_table) {
        int kp_count = task_kp_count(task);
        if (kp_count == MAX_PAGE_COUNT) {
            free_page(pmd);
            return -1;
        }
        task->mm.kernel_pages[kp_count] = pmd;
    }

    u64 pte = map_table((u64*)PA_TO_KVA(pmd), PMD_SHIFT, uva, &new_table);
    if (new_table) {
        int kp_count = task_kp_count(task);
        if (kp_count == MAX_PAGE_COUNT) {
            free_page(pte);
            return -1;
        }
        task->mm.kernel_pages[kp_count] = pte;
    }

    map_table_entry((u64*)PA_TO_KVA(pte), uva, phys_page);
    int up_count = task_up_count(task);
    if (up_count == MAX_PAGE_COUNT)
        return -1;
    struct user_page up = {phys_page, uva};
    task->mm.user_pages[up_count] = up;
    return 0;
}

/* 
 * args: 
 * @table: kva of table to fill an entry
 * @shift: shift used to turn va into index
 * @va: the va we want to map
 * @new_table: whether the entry is already filled and the next level allocated already
 *
 * returns: pa of the next level
 */
u64 map_table(u64 *table, u64 shift, u64 uva, int *new_table)
{
    u64 index = uva >> shift;
    index = index & (ENTRIES_PER_TABLE - 1);
    if (!table[index]) {
        *new_table = 1;
        u64 next_level_table = get_free_page();
        u64 entry = next_level_table | TD_USER_TABLE_FLAGS;
        table[index] = entry;
        return next_level_table;
    }
    else {
        *new_table = 0;
        return table[index] & PAGE_MASK;
    }
}

void map_table_entry(u64 *pte, u64 uva, u64 phys_page)
{
    u64 index = uva >> PAGE_SHIFT;
    index = index & (ENTRIES_PER_TABLE - 1);
    u64 entry = phys_page | TD_USER_PAGE_FLAGS;
    pte[index] = entry;
}

/* copy current's memory space to dst */
int copy_virt_memory(struct task_struct *dst)
{
    for (int i = 0; i < task_up_count(current); i++) {
        u64 kva = allocate_user_page(dst, current->mm.user_pages[i].uva);
        if (kva == 0)
            return -1;
        memcpy((u64*)kva, (u64*)(current->mm.user_pages[i].uva), PAGE_SIZE);
    }
    return 0;
}

/* utility function to get used count of kernel pages array */
int task_kp_count(struct task_struct *task)
{
    for (int i = 0; i < MAX_PAGE_COUNT; i++) {
        if (task->mm.kernel_pages[i] == 0)
            return i;
    }
    return MAX_PAGE_COUNT;
}

/* utility function to get used count of user pages array */
int task_up_count(struct task_struct *task)
{
    for (int i = 0; i < MAX_PAGE_COUNT; i++) {
        if (task->mm.user_pages[i].pa == 0)
            return i;
    }
    return MAX_PAGE_COUNT;
}

/* returns a kva of a free page */
u64 get_kernel_page()
{
    u64 phys_page = get_free_page();
    return PA_TO_KVA(phys_page);
}

void free_kernel_page(u64 kp)
{
    free_page(KVA_TO_PA(kp));
}

/* returns a pa of a free page, does not return if it fails */
u64 get_free_page()
{
    for (int i = 0; i < MALLOC_PAGES; i++) {
        if (mem_map[i] == 0) {
            mem_map[i] = 1;
            u64 ret = MALLOC_START + i * PAGE_SIZE;
            memzero(PA_TO_KVA(ret), PAGE_SIZE);
            return ret;
        }
    }
    panic("Out of physical memory!");
    /* doesn't comeback */
    return 0;
}

void free_page(u64 p)
{
    mem_map[(p - MALLOC_START) / PAGE_SIZE] = 0;
}

int do_data_abort(u64 far, u64 esr)
{
    u64 dfsc = esr & 0x3f;
    /* 0x7: level 3 translation fault */
    if (dfsc == 0x7) {
        u64 page = get_free_page();
        if (map_page(current, far & PAGE_MASK, page) < 0)
            return -1;
        return 0;
    }
    return -1;
}

