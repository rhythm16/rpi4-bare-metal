#include "peripherals/timer.h"
#include "peripherals/irq.h"
#include "peripherals/aux.h"
#include "irq_numbers.h"
#include "mini_uart.h"
#include "generic_timer.h"
#include "utils.h"

const u32 interval_1 = CLOCKHZ;
u32 cur_ls32_1 = 0;

/* we can only use system timer compare 1 and 3, 0 and 2 are used by the vc. */
void timer_init()
{
    cur_ls32_1 = SYS_REGS_TIMER->counter_lo;
    cur_ls32_1 += interval_1;
    SYS_REGS_TIMER->compare[1] = cur_ls32_1;
}

void handle_sys_timer_1()
{
    cur_ls32_1 += interval_1;
    SYS_REGS_TIMER->compare[1] = cur_ls32_1;
    SYS_REGS_TIMER->countrol_status |= (1 << 1);
    uart_send_string("timer 1 interrupt\n");
    u64 sys_count = get_sys_count();
    char printable[17];
    u64_to_char_array(sys_count, printable);
    printable[16] = '\0';
    uart_send_string(printable);
    uart_send_string("\n");
}