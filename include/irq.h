#ifndef IRQ_H
#define IRQ_H

#include "types.h"

void irq_init_vectors();
void irq_enable();
void irq_disable();

void enable_interrupt_gic(u32 INTID, u32 core);

#endif /* IRQ_H */
