.globl memzero
memzero:
    /* store 8 bytes of zero to [x0], then x0 += 8 */
    str xzr, [x0], #8
    subs x1, x1, #8
    /* branch if greater than zero */
    b.gt memzero
    ret
    
