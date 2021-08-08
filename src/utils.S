.globl delay
delay:
    /* s trailing means set the flags */
    subs x0, x0, #1
    bne delay
    ret

.globl put32
put32:
    str w1, [x0]
    ret

.globl get32
get32:
    ldr w0, [x0]
    ret

.globl get_el
get_el:
    mrs x0, CurrentEL
    lsr x0, x0, #2
    ret

.globl get_core
get_core:
    mrs x0, mpidr_el1
    and x0, x0, #0xFF
    ret
