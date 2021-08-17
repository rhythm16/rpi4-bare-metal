#ifndef IRQ_NUMBERS_H 
#define IRQ_NUMBERS_H

/* vc irq numbers */
enum gic_irqs
{
    NS_PHYS_TIMER_IRQ = 30,
    VC_TIMER_IRQ_0    = 96,
    VC_TIMER_IRQ_1    = 97, 
    VC_TIMER_IRQ_2    = 98, 
    VC_TIMER_IRQ_3    = 99,
    VC_AUX_IRQ        = 125
};

#endif /* IRQ_NUMBERS_H */
