#include "utils.h"
#include "types.h"
#include "fork.h"
#include "mm.h"
#include "sched.h"
#include "sys.h"

void * sys_call_table[] = {sys_write, sys_get_free_page, sys_clone, sys_exit};

void sys_write(char * buf)
{
    main_output(MU, buf);
}

int sys_clone(u64 stack)
{
    /* we are cloning, so fn and arg do not need to be provided */
    return copy_process(UTHREAD, 0, 0, stack);
}

u64 sys_get_free_page()
{
    u64 addr = get_free_page();
    if (!addr)
        return -1;
    return addr;
}

void sys_exit()
{
    exit_process();
}

void sys_call_table_relocate()
{
    for (int i = 0; i < NR_SYSCALLS; i++) {
        sys_call_table[i] = (void *)((u64)sys_call_table[i] + (u64)KERNEL_START);
    }
}

