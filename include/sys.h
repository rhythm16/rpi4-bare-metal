#ifndef SYS_H
#define SYS_H

#define NR_SYSCALLS  4

#define SYS_WRITE_NUM          0
#define SYS_GET_FREE_PAGE_NUM  1
#define SYS_CLONE_NUM          2
#define SYS_EXIT_NUM           3

#ifndef __ASSEMBLER__

#include "types.h"

void sys_write(char * buf);
int sys_clone(u64 stack);
u64 sys_get_free_page();
void sys_exit();

void call_sys_write(char * buf);
int call_sys_clone(u64 fn, u64 arg, u64 stack);
u64 call_sys_get_free_page();
void call_sys_exit();

void sys_call_table_relocate();

#endif /* __ASSEMBLER__ */

#endif /* SYS_H */
