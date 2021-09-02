/* version: 2020/09/25 */
/*
 * Copyright (c) 2016-2019 Raspberry Pi (Trading) Ltd.
 * Copyright (c) 2016 Stephen Warren <swarren@wwwdotorg.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * * Neither the name of the copyright holder nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sysregs.h"

/* RPI4 with no HIGH_PERI set */
#define BCM2711 1
#undef HIGH_PERI

#if BCM2711
#ifdef HIGH_PERI
#define LOCAL_CONTROL       0x4c0000000
#define LOCAL_PRESCALER     0x4c0000008
#else
#define LOCAL_CONTROL       0xff800000
#define LOCAL_PRESCALER     0xff800008
#endif
#else
#define LOCAL_CONTROL       0x40000000
#define LOCAL_PRESCALER     0x40000008
#endif

#ifdef HIGH_PERI
#define GIC_DISTB       0x4c0041000
#define GIC_CPUB        0x4c0042000
#else
#define GIC_DISTB       0xff841000
#define GIC_CPUB        0xff842000
#endif

#if BCM2711
#define OSC_FREQ        54000000
#else
#define OSC_FREQ        19200000
#endif


#define GICC_CTRLR  0x0
#define GICC_PMR    0x4
#define IT_NR       0x8 // Number of interrupt enable registers (256 total irqs)
#define GICD_CTRLR  0x0
#define GICD_IGROUPR    0x80

.globl _start
_start:
    /*
     * LOCAL_CONTROL:
     * Bit 9 clear: Increment by 1 (vs. 2).
     * Bit 8 clear: Timer source is 19.2MHz crystal (vs. APB).
     */
    ldr x0, =LOCAL_CONTROL
    str wzr, [x0]
    /* LOCAL_PRESCALER; divide-by (0x80000000 / register_val) == 1 */
    mov w1, 0x80000000
    str w1, [x0, #(LOCAL_PRESCALER - LOCAL_CONTROL)]

    /* Set L2 read/write cache latency to 3 */
    mrs x0, L2CTLR_EL1
    mov x1, #0x22
    orr x0, x0, x1
    msr L2CTLR_EL1, x0

    /* Set up CNTFRQ_EL0 */
    ldr x0, =OSC_FREQ
    msr CNTFRQ_EL0, x0

    /* Set up CNTVOFF_EL2 */
    /* physical timer value = virtual timer value */
    msr CNTVOFF_EL2, xzr

    /* Enable FP/SIMD */
    /* bit 10 (TFP) is set to 0, others are res0 */
    msr CPTR_EL3, xzr

    /* Set up SCR */
    mov x0, #SCR_VAL
    msr SCR_EL3, x0

    /* Set up ACTLR */
    mov x0, #ACTLR_VAL
    msr ACTLR_EL3, x0

    /* Set SMPEN */
    mov x0, #CPUECTLR_EL1_SMPEN
    msr CPUECTLR_EL1, x0

    bl      setup_gic

    /*
     * Set up SCTLR_EL2
     * All set bits below are res1. LE, no WXN/I/SA/C/A/M
     */
    ldr x0, =0x30c50830
    msr SCTLR_EL2, x0

    /* setup SCTLR_EL1 */
    ldr x0, =SCTLR_EL1_VAL_MMU_DISABLED
    msr SCTLR_EL1, x0

    /* setup HCR_EL2 */
    ldr x0, =HCR_EL2_VAL
    msr HCR_EL2, x0

    /* setup SPSR_EL3 */
    ldr x0, =SPSR_EL3_VAL
    msr SPSR_EL3, x0

    /* setup CPACR_EL1 */
    ldr x0, =CPACR_EL1_VAL
    msr CPACR_EL1, x0

    mrs x6, MPIDR_EL1
    and x6, x6, #0x3
    cbz x6, primary_cpu

    adr x5, spin_cpu0
secondary_spin:
    wfe
    ldr x4, [x5, x6, lsl #3]
    cbz x4, secondary_spin
    mov x0, #0
    b boot_kernel

.ltorg

.org 0xd8
.globl spin_cpu0
spin_cpu0:
    .quad 0
.org 0xe0
.globl spin_cpu1
spin_cpu1:
    .quad 0
.org 0xe8
.globl spin_cpu2
spin_cpu2:
    .quad 0
.org 0xf0
.globl spin_cpu3
spin_cpu3:
    # Shared with next two symbols/.word
    # FW clears the next 8 bytes after reading the initial value, leaving
    # the location suitable for use as spin_cpu3
.org 0xf0
.globl stub_magic
stub_magic:
    .word 0x5afe570b
.org 0xf4
.globl stub_version
stub_version:
    .word 0
.org 0xf8
.globl dtb_ptr32
dtb_ptr32:
    .word 0x0
.org 0xfc
.globl kernel_entry32
kernel_entry32:
    .word 0x0

.org 0x100

primary_cpu:
    ldr w4, kernel_entry32
    ldr w0, dtb_ptr32

boot_kernel:
    mov x1, #0
    mov x2, #0
    mov x3, #0
    br x4

setup_gic:              // Called from secure mode - set all interrupts to group 1 and enable.
    mrs x0, MPIDR_EL1
    ldr x2, =GIC_DISTB
    tst x0, #0x3
    b.eq    2f          // primary core

    mov w0, #3          // Enable group 0 and 1 IRQs from distributor
    str w0, [x2, #GICD_CTRLR]
2:
    add x1, x2, #(GIC_CPUB - GIC_DISTB)
    mov w0, #0x1e7
    str w0, [x1, #GICC_CTRLR]   // Enable group 1 IRQs from CPU interface
    mov w0, #0xff
    str w0, [x1, #GICC_PMR] // priority mask
    add x2, x2, #GICD_IGROUPR
    mov x0, #(IT_NR * 4)
    mov w1, #~0         // group 1 all the things
3:
    subs    x0, x0, #4
    str w1, [x2, x0]
    b.ne    3b
    ret

.globl dtb_space
dtb_space:

