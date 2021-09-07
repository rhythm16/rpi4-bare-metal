#ifndef MMU_H
#define MMU_H

/* translation table descriptors */
#define TD_VALID                   (1 << 0)
#define TD_BLOCK                   (0 << 1)
#define TD_PAGE                    (1 << 1)
#define TD_TABLE                   (1 << 1)
/* 
 * we can check ID_AA64MMFR1_EL1.HAFDBS to see if the hardware supports hw management
 * of the access flag and dirty state, but we'll just set the access flag to 1 by sw. 
 */
#define TD_ACCESS                  (1 << 10)
/* D5-2739 */
/* EL1 rwx, EL0 - */
#define TD_KERNEL_PERMS            (1 << 54)
/* EL1 rw, EL0 rwx */
#define TD_USER_PERMS              (1 << 6)
/*
 * memory region shared by all cores, but this has no effect now because non-cachable and
 * device memory are always outer sharable
 * */
#define TD_INNER_SHARABLE          (3 << 8)

#define TD_KERNEL_TABLE_FLAGS      (TD_TABLE | TD_VALID)
#define TD_KERNEL_BLOCK_FLAGS      (TD_ACCESS | TD_INNER_SHARABLE | TD_KERNEL_PERMS | (MATTR_NORMAL_NC_INDEX << 2) | TD_BLOCK | TD_VALID)
#define TD_DEVICE_BLOCK_FLAGS      (TD_ACCESS | TD_INNER_SHARABLE | TD_KERNEL_PERMS | (MATTR_DEVICE_nGnRnE_INDEX << 2) | TD_BLOCK | TD_VALID)
#define TD_USER_TABLE_FLAGS        (TD_TABLE | TD_VALID)
#define TD_USER_PAGE_FLAGS         (TD_ACCESS | TD_INNER_SHARABLE | TD_USER_PERMS | (MATTR_NORMAL_NC_INDEX << 2) | TD_PAGE | TD_VALID)

/* memory attribute indirect register */
#define MATTR_DEVICE_nGnRnE        0x0
#define MATTR_NORMAL_NC            0x44
#define MATTR_DEVICE_nGnRnE_INDEX  0
#define MATTR_NORMAL_NC_INDEX      1
#define MAIR_EL1_VAL               ((MATTR_NORMAL_NC << (8 * MATTR_NORMAL_NC_INDEX)) | MATTR_DEVICE_nGnRnE << (8 * MATTR_DEVICE_nGnRnE_INDEX))

/* translation control register */
#define TCR_TG1_4K     (2 << 30)
#define TCR_T1SZ       ((64 - 48) << 16)
#define TCR_TG0_4K     (0 << 14)
#define TCR_T0SZ       (64 - 48)
#define TCR_EL1_VAL    (TCR_TG1_4K | TCR_T1SZ | TCR_TG0_4K | TCR_T0SZ)

#endif /* MMU_H */
