#include "sys.h"

.globl call_sys_write
call_sys_write:
    mov w8, #SYS_WRITE_NUM
    svc #0
    ret

.globl call_sys_exit
call_sys_exit:
    mov w8, #SYS_EXIT_NUM
    svc #0
    ret

.globl call_sys_fork
call_sys_fork:
    mov w8, #SYS_FORK_NUM
    svc #0
    ret
