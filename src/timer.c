#include "peripherals/timer.h"
#include "peripherals/irq.h"
#include "peripherals/aux.h"
#include "mini_uart.h"

const u32 interval_1 = CLOCKHZ;
u32 cur_val_1 = 0;

void timer_init()
{
    cur_val_1 = SYS_REGS_TIMER->counter_lo;
    cur_val_1 += interval_1;
    SYS_REGS_TIMER->compare[1] = cur_val_1;
}

void handle_sys_timer_1()
{
    cur_val_1 += interval_1;
    SYS_REGS_TIMER->compare[1] = cur_val_1;
    SYS_REGS_TIMER->countrol_status |= SYS_TIMER_IRQ_1;
    uart_send_string("timer 1 interrupt\n");
}
