/*
 * when we want to trace, the cores will bl to here
 */
.globl hook
hook:
/*
    mov x10, lr
    mov lr, x9
    br  x10
*/
    sub sp, sp, #96 /* create space */

    /* save function arguments, return address, callee address and fp */
    stp x0, x1,    [sp, #16 * 0]
    stp x2, x3,    [sp, #16 * 1]
    stp x4, x5,    [sp, #16 * 2]
    stp x6, x7,    [sp, #16 * 3]
    stp x8, x9,    [sp, #16 * 4]
    stp x29, x30,  [sp, #16 * 5]

    /* prepare arguments and call into C */
    bl traced
    
    /* restore function arguments and fp, although the fp shouldn't change */
    ldp x0, x1,    [sp, #16 * 0]
    ldp x2, x3,    [sp, #16 * 1]
    ldp x4, x5,    [sp, #16 * 2]
    ldp x6, x7,    [sp, #16 * 3]
    ldp x8, x9,    [sp, #16 * 4]
    ldp x29, x30,  [sp, #16 * 5]

    /* execute the function */
    blr x30

    /* return to caller */
    ldr x30,       [sp, #16 * 4 + 8]

    /* restore sp position */
    add sp, sp, #96

    ret

