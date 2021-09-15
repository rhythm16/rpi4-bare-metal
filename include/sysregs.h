#ifndef SYSREGS_H
#define SYSREGS_H

#define BIT(x) (1 << (x))

/* security control register */
#define SCR_RW			BIT(10) // set next lower exception levels to aarch64 p.3571
#define SCR_HCE			BIT(8)  // hypervisor call enable p.3572
#define SCR_SMD			BIT(7)  // secure monitor call disable p.3573
#define SCR_RES1_5		BIT(5)  // reserved as 1 p.3573
#define SCR_RES1_4		BIT(4)  // reserved as 1 p.3573
#define SCR_NS			BIT(0)  // non-secure bit, EL0 and EL1 are non-secure p.3574
#define SCR_VAL \
    (SCR_RW | SCR_HCE | SCR_SMD | SCR_RES1_5 | SCR_RES1_4 | SCR_NS)

/* auxiliary control register for EL3 */
/* this is implementation defined, so look into the Cortex A72 TRM */
/* make CPU auxiliary, extended registers and L2 auxiliary, control register */
/* accessible from a lower exception level */
#define ACTLR_VAL \
	(BIT(0) | BIT(1) | BIT(4) | BIT(5) | BIT(6))

/* name for CPUECTLR_EL1 is S3_1_C15_C2_1 in Aarch64 */
/* also implementation defined */
#define CPUECTLR_EL1		S3_1_C15_C2_1
/* enables receiving of instruction cache and TLB maintenance operations */
/* must be enabled before using caches or MMU */
#define CPUECTLR_EL1_SMPEN	BIT(6)

/* Holds the saved process state when an exception is taken to EL3 */
#define SPSR_EL3_D		BIT(9) // debug exception mask
#define SPSR_EL3_A		BIT(8) // SError interrupt mask
#define SPSR_EL3_I		BIT(7) // IRQ interrupt mask
#define SPSR_EL3_F		BIT(6) // FIQ interrupt mask
#define SPSR_EL3_MODE_EL3H	13 // AArch64 Exception level and selected Stack Pointer, 13 is EL.3h.
#define SPSR_EL3_MODE_EL1H  5  // AArch64 Exception level and selected Stack Pointer, 5 is EL.1h.
#define SPSR_EL3_VAL \
    (SPSR_EL3_D | SPSR_EL3_A | SPSR_EL3_I | SPSR_EL3_F | SPSR_EL3_MODE_EL1H)

/* Holds the saved process state when an exception is taken to EL1 */
#define SPSR_EL1_MODE_EL0t  0  // AArch64 Exception level and selected Stack Pointer, 0 is EL.0t.

/* name for L2CTLR_EL1 is S3_1_C11_C0_2 in Aarch64 */
#define L2CTLR_EL1		S3_1_C11_C0_2

#define SCTLR_EL1_RESERVED           (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)
#define SCTLR_EL1_EE_LITTLE_ENDIAN   (0 << 25)
#define SCTLR_EL1_EOE_LITTLE_ENDIAN  (0 << 24)
#define SCTLR_EL1_I_CACHE_DISABLED   (0 << 12)
#define SCTLR_EL1_D_CACHE_DISABLED   (0 << 2)
#define SCTLR_EL1_MMU_DISABLED       (0 << 0)
#define SCTLR_EL1_MMU_ENABLED        (1 << 0)

#define SCTLR_EL1_VAL_MMU_DISABLED (SCTLR_EL1_RESERVED | SCTLR_EL1_EE_LITTLE_ENDIAN | SCTLR_EL1_I_CACHE_DISABLED | SCTLR_EL1_D_CACHE_DISABLED | SCTLR_EL1_MMU_DISABLED)

/* hypervisor control register */
#define HCR_EL2_RW                   BIT(31) // EL1 is AArch64
#define HCR_EL2_VAL                  HCR_EL2_RW

/* architectural feature access control register */
#define CPACR_EL1_FPEN    BIT(21) | BIT(20) // don't trap SIMD/FP registers
#define CPACR_EL1_ZEN     BIT(17) | BIT(16)  // don't trap SVE instructions
#define CPACR_EL1_VAL     (CPACR_EL1_FPEN | CPACR_EL1_ZEN)

/* exception syndrome register EL1 (ESR_EL1) */
#define ESR_ELx_EC_SHIFT 26
#define ESR_ELx_EC_SVC64 0x15
#define ESR_ELx_EC_DA_LOW 0x24

#endif /* SYSREGS_H */
