#ifndef PER_IRQ_H
#define PER_IRQ_H

#include "peripherals/base.h"
#include "types.h"

enum vc_irqs {
    AUX_IRQ = (1 << 29)
};

/* core 0's videocore interrupt control */
struct arm_irq_regs_2711 {
    reg32 irq_pending_0;
    reg32 irq_pending_1;
    reg32 irq_pending_2;
    reg32 res0;
    reg32 irq_enable_0;
    reg32 irq_enable_1;
    reg32 irq_enable_2;
    reg32 res1;
    reg32 irq_disable_0;
    reg32 irq_disable_1;
    reg32 irq_disable_2;
};

#define REGS_IRQ0 ((struct arm_irq_regs_2711 *)(PBASE + 0xb200))

#endif /* PER_IRQ_H */
