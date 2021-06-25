#include "mm.h"

.section ".text.boot"

.globl _start
_start:
    /* mpidr_el1 is the multiprocessor affinity register */
    mrs x0, mpidr_el1
    and x0, x0, #0xFF
    /* compare and branch */
    cbz x0, master
    /* 
     * Hang if x0 is not 0, meaning we're not running on core 0.
     * All the cores will run this code, hang all of them except
     * core 0.
     */
    b proc_hang

master:
    /* adr is load address */
    adr x0, bss_begin
    adr x1, bss_end
    sub x1, x1, x0
    /* clear out the bss section */
    /* branch and link, x30 is set to the address of the next instruction */
    bl memzero
    mov sp, #LOW_MEMORY
    bl kernel_main
    /* shouldn't get here */
    b proc_hang

proc_hang:
    /* wait for event */
    wfe
    b proc_hang
