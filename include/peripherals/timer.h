#ifndef PER_TIMER_H
#define PER_TIMER_H

#include "peripherals/base.h"
#include "types.h"

#define CLOCKHZ 1000000

struct sys_timer_regs {
    reg32 countrol_status;
    reg32 counter_lo;
    reg32 counter_hi;
    reg32 compare[4];
};

#define SYS_REGS_TIMER ((struct sys_timer_regs *)(PBASE + 0x3000))
#endif /* PER_TIMER_H */
