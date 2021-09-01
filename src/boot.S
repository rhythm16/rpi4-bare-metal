#include "mm.h"
#include "sysregs.h"

.section ".text.boot"

.globl _start
_start:
    /* only core 0 starts here */
    b master
    /* shouldn't come here */
    b proc_hang

master: /* entry point of the primary core */
    bl drop_to_el1
    mov sp, #LOW_MEMORY
    /* adr is load address */
    adr x0, bss_begin
    adr x1, bss_end
    sub x1, x1, x0
    /* clear out the bss section */
    /* branch and link, x30 is set to the address of the next instruction */
    bl memzero
    bl wake_up_cores
    /* save kernel pa base */
    adr x0, _start
    adr x1, KERNEL_PA_BASE
    str x0, [x1]
    /* we are core 0 */
    mov x0, #0
    bl kernel_main
    /* shouldn't get here */
    b proc_hang

app: /* entry point of the secondary cores */
    bl drop_to_el1
    /* setup stack */
    mrs x0, mpidr_el1
    and x0, x0, #0xFF
    mov x1, #SECTION_SIZE
    mul x1, x1, x0
    add x1, x1, #LOW_MEMORY
    mov sp, x1
    bl kernel_main

drop_to_el1:
    adr x0, el1_entry
    msr ELR_EL3, x0
    eret
el1_entry:
    ret

wake_up_cores:
    sev
    mov x0, #0
    adr x0, app
    mov x1, #0xe0
    str x0, [x1]
    mov x1, #0xe8
    str x0, [x1]
    mov x1, #0xf0
    str x0, [x1]
    ret

proc_hang:
    /* wait for event */
    wfe
    b proc_hang
