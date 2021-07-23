#ifndef IRQ_H
#define IRQ_H

void irq_init_vectors();
void irq_enable();
void irq_disable();

void enable_core0_interrupt_controller(int count, int *irq_numbers);

#endif /* IRQ_H */
