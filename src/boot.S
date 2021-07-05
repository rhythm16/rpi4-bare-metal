#include "mm.h"
#include "sysregs.h"

.section ".text.boot"

.globl _start
_start:
    /* only core 0 starts here */
    b master
    /* shouldn't come here */
    b proc_hang

master:
    /* setup SCTLR_EL1 */
    ldr x0, =SCTLR_EL1_VAL_MMU_DISABLED
    msr SCTLR_EL1, x0

    /* setup HCR_EL2 */
    ldr x0, =HCR_EL2_VAL
    msr HCR_EL2, x0

    /* setup SPSR_EL3 */
    ldr x0, =SPSR_EL3_VAL
    msr SPSR_EL3, x0

    adr x0, el1_entry
    msr ELR_EL3, x0
    eret

el1_entry:
    mov sp, #LOW_MEMORY
    /* adr is load address */
    adr x0, bss_begin
    adr x1, bss_end
    sub x1, x1, x0
    /* clear out the bss section */
    /* branch and link, x30 is set to the address of the next instruction */
    bl memzero
    bl wake_up_cores
    /* we are core 0 */
    mov x0, #0
    bl kernel_main
    /* shouldn't get here */
    b proc_hang

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

app: /* entry point of the secondary cores */
    /* setup stack */
    mrs x0, mpidr_el1
    and x0, x0, #0xFF
    mov x1, #SECTION_SIZE
    mul x1, x1, x0
    add x1, x1, #LOW_MEMORY
    mov sp, x1
    bl kernel_main

proc_hang:
    /* wait for event */
    wfe
    b proc_hang
