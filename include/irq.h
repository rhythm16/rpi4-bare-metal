#ifndef IRQ_H
#define IRQ_H

void irq_init_vectors();
void irq_enable();
void irq_disable();

void enable_core0_interrupt_controller_AUX();

#endif /* IRQ_H */
