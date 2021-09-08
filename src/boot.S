#include "mm.h"
#include "mmu.h"
#include "sysregs.h"

/* changes tmp1, tmp2 only */
.macro create_table_entry, tbl, ntbl, va, shift, flags, tmp1, tmp2
    /* get entry index in tmp1 */
    lsr \tmp1, \va, #\shift
    and \tmp1, \tmp1, #ENTRIES_PER_TABLE - 1
    /* tmp2 = entry value */
    mov \tmp2, \ntbl
    orr \tmp2, \tmp2, #\flags
    /* install entry */
    str \tmp2, [\tbl, \tmp1, lsl #3]
.endm

/* changes vstart, vend, pa, tmp1 */
/* vstart and vend should not point to the same block! */
.macro create_block_map, pmd, vstart, vend, pa, flags, tmp1
    /* turn vstart, vend into indices */
    lsr \vstart, \vstart, #SECTION_SHIFT
    and \vstart, \vstart, #ENTRIES_PER_TABLE - 1
    lsr \vend, \vend, #SECTION_SHIFT
    /* minus one to handle the last entry */
    sub \vend, \vend, #1
    and \vend, \vend, #ENTRIES_PER_TABLE - 1
    /* loop init, pa = pa | flags */
    lsr \pa, \pa, #SECTION_SHIFT
    lsl \pa, \pa, #SECTION_SHIFT
    ldr \tmp1, =\flags
    orr \pa, \pa, \tmp1
    /* loop */
    /* pmd[vstart] = pa */
2:
    str \pa, [\pmd, \vstart, lsl #3]
    /* pa += section size */
    add \pa, \pa, #SECTION_SIZE
    /* vstart += 1 */
    add \vstart, \vstart, #1
    cmp \vstart, \vend
    b.le 2b
.endm

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
    bl map_identity
    bl map_high
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

map_identity:
    /* save return address */
    mov x29, x30
    adrp x0, id_pg_dir
    mov x1, #ID_MAP_TABLE_SIZE
    /* clear id page tables */
    bl memzero
    adrp x0, id_pg_dir
    /* x1 = address of id map pud */
    add x1, x0, #PAGE_SIZE
    eor x4, x4, x4
    /* install PGD entry */
    create_table_entry x0, x1, x4, PGD_SHIFT, TD_KERNEL_TABLE_FLAGS, x2, x3
    /* goto next level */
    add x0, x0, #PAGE_SIZE
    add x1, x1, #PAGE_SIZE
    /* install PUD entry */
    create_table_entry x0, x1, x4, PUD_SHIFT, TD_KERNEL_TABLE_FLAGS, x2, x3
    /* id map 0-16M */
    mov x0, x1
    eor x2, x2, x2
    ldr x3, =ID_MAP_SIZE
    eor x4, x4, x4
    create_block_map x0, x2, x3, x4, TD_KERNEL_BLOCK_FLAGS, x5
    
    /* restore return address */
    mov x30, x29
    ret

map_high:
    /* save return address */
    mov x29, x30
    adrp x0, high_pg_dir
    mov x1, #HIGH_MAP_TABLE_SIZE
    /* clear high page tables */
    bl memzero
    adrp x0, high_pg_dir
    /* x1 = address of high map pud */
    add x1, x0, #PAGE_SIZE
    /* x4 = address of va we map (pgd) */
    ldr x4, =LINEAR_MAP_BASE
    /* install PGD entry */
    create_table_entry x0, x1, x4, PGD_SHIFT, TD_KERNEL_TABLE_FLAGS, x2, x3
    /* goto next level */
    add x0, x0, #PAGE_SIZE
    add x1, x1, #PAGE_SIZE
    /* x4 = address of va we map (pud) */
    ldr x4, =LINEAR_MAP_BASE
    ldr x5, =PUD_ENTRY_MAP_SIZE
    /* install first PUD entry */
    create_table_entry x0, x1, x4, PUD_SHIFT, TD_KERNEL_TABLE_FLAGS, x2, x3
    add x1, x1, #PAGE_SIZE
    add x4, x4, x5
    create_table_entry x0, x1, x4, PUD_SHIFT, TD_KERNEL_TABLE_FLAGS, x2, x3
    add x1, x1, #PAGE_SIZE
    add x4, x4, x5
    create_table_entry x0, x1, x4, PUD_SHIFT, TD_KERNEL_TABLE_FLAGS, x2, x3
    add x1, x1, #PAGE_SIZE
    add x4, x4, x5
    create_table_entry x0, x1, x4, PUD_SHIFT, TD_KERNEL_TABLE_FLAGS, x2, x3
    /* load some values */
    ldr x10, =HIGH_MAP_FIRST_START
    ldr x11, =HIGH_MAP_FIRST_END
    ldr x12, =HIGH_MAP_SECOND_START
    ldr x13, =HIGH_MAP_SECOND_END
    ldr x14, =HIGH_MAP_THIRD_START
    ldr x15, =HIGH_MAP_THIRD_END
    ldr x16, =HIGH_MAP_FOURTH_START
    ldr x17, =HIGH_MAP_FOURTH_END
    ldr x18, =HIGH_MAP_DEVICE_START
    ldr x19, =HIGH_MAP_DEVICE_END
    ldr x20, =FIRST_START
    ldr x21, =SECOND_START
    ldr x22, =THIRD_START
    ldr x23, =FOURTH_START
    ldr x24, =DEVICE_START
    /* map first high part */
    add x0, x0, #PAGE_SIZE
    mov x2, x10
    mov x3, x11
    mov x4, x20
    create_block_map x0, x2, x3, x4, TD_KERNEL_BLOCK_FLAGS, x5
    /* map second high part */
    add x0, x0, #PAGE_SIZE
    mov x2, x12
    mov x3, x13
    mov x4, x21
    create_block_map x0, x2, x3, x4, TD_KERNEL_BLOCK_FLAGS, x5
    /* map third high part */
    add x0, x0, #PAGE_SIZE
    mov x2, x14
    mov x3, x15
    mov x4, x22
    create_block_map x0, x2, x3, x4, TD_KERNEL_BLOCK_FLAGS, x5
    /* map fourth high part */
    add x0, x0, #PAGE_SIZE
    mov x2, x16
    mov x3, x17
    mov x4, x23
    create_block_map x0, x2, x3, x4, TD_KERNEL_BLOCK_FLAGS, x5
    /* map device */
    mov x2, x18
    mov x3, x19
    mov x4, x24
    create_block_map x0, x2, x3, x4, TD_DEVICE_BLOCK_FLAGS, x5
    
    /* restore return address */
    mov x30, x29
    ret

proc_hang:
    /* wait for event */
    wfe
    b proc_hang

