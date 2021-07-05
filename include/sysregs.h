#ifndef SYSREGS_H
#define SYSREGS_H

#define SCTLR_EL1_RESERVED           (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)
#define SCTLR_EL1_EE_LITTLE_ENDIAN   (0 << 25)
#define SCTLR_EL1_EOE_LITTLE_ENDIAN  (0 << 24)
#define SCTLR_EL1_I_CACHE_DISABLED   (0 << 12)
#define SCTLR_EL1_D_CACHE_DISABLED   (0 << 2)
#define SCTLR_EL1_MMU_DISABLED       (0 << 0)
#define SCTLR_EL1_MMU_ENABLED        (1 << 0)

#define SCTLR_EL1_VAL_MMU_DISABLED (SCTLR_EL1_RESERVED | SCTLR_EL1_EE_LITTLE_ENDIAN | SCTLR_EL1_I_CACHE_DISABLED | SCTLR_EL1_D_CACHE_DISABLED | SCTLR_EL1_MMU_DISABLED)

#define HCR_EL2_RW                   (1 << 31)
#define HCR_EL2_VAL                  HCR_EL2_RW

#define SPSR_EL3_D		             (1 << 9) // debug exception mask
#define SPSR_EL3_A		             (1 << 8) // SError interrupt mask
#define SPSR_EL3_I		             (1 << 7) // IRQ interrupt mask
#define SPSR_EL3_F		             (1 << 6) // FIQ interrupt mask
#define SPSR_EL3_MODE_EL1H	               5  // AArch64 Exception level and selected Stack Pointer, 5 is EL.1h.
#define SPSR_EL3_VAL \
    (SPSR_EL3_D | SPSR_EL3_A | SPSR_EL3_I | SPSR_EL3_F | SPSR_EL3_MODE_EL1H)

#endif /* SYSREGS_H */
