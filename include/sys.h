#ifndef SYS_H
#define SYS_H

#define NR_SYSCALLS  3

#define SYS_WRITE_NUM          0
#define SYS_FORK_NUM           1
#define SYS_EXIT_NUM           2

#ifndef __ASSEMBLER__

#include "types.h"

void sys_write(char * buf);
int sys_fork();
void sys_exit();

void call_sys_write(char * buf);
int call_sys_fork();
void call_sys_exit();

void sys_call_table_relocate();

#endif /* __ASSEMBLER__ */

#endif /* SYS_H */
