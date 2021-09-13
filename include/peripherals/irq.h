#ifndef PER_IRQ_H
#define PER_IRQ_H

#include "types.h"
#include "mm.h"

#define GIC_BASE            PA_TO_KVA(0xFF840000)
#define GICD_BASE           (GIC_BASE + 0x1000)
#define GICC_BASE           (GIC_BASE + 0x2000)

#define GICD_ISENABLER_BASE (GICD_BASE + 0x100)
#define GICD_ITARGETSR_BASE (GICD_BASE + 0x800)

#define GICC_IAR            (GICC_BASE + 0x0C)
#define GICC_EOIR           (GICC_BASE + 0x10)

struct distributor_enable_regs
{
    reg32 bitmap[32];
};

struct distributor_target_regs
{
    reg32 set[255];
};

#define GICD_ISENABLERN     ((struct distributor_enable_regs *)(GICD_ISENABLER_BASE))
#define GICD_ITARGETSRN     ((struct distributor_target_regs *)(GICD_ITARGETSR_BASE))

#endif /* PER_IRQ_H */
