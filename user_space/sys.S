#include "sys.h"

.globl call_sys_write
call_sys_write:
    mov w8, #SYS_WRITE_NUM
    svc #0
    ret

.globl call_sys_get_free_page
call_sys_get_free_page:
    mov w8, #SYS_GET_FREE_PAGE_NUM
    svc #0
    ret

.globl call_sys_exit
call_sys_exit:
    mov w8, #SYS_EXIT_NUM
    svc #0
    ret

.globl call_sys_clone
call_sys_clone:
    mov x10, x0  /* function to run */
    mov x11, x1  /* argument */
    mov x12, x2  /* user stack page */

    mov x0, x2
    mov w8, #SYS_CLONE_NUM
    svc #0

    /* check the return value, if it is not 0, then we're the parent, just return.
     * if it is 0, then we're the child, run thread_start */
    cmp x0, #0
    beq thread_start
    ret

thread_start:
    mov x29, 0

    /* load argument and run the target function */
    mov x0, x11
    blr x10

    /* exit the task if we return */
    mov x8, #SYS_EXIT_NUM
    svc #0

