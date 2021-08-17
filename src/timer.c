#include "peripherals/timer.h"
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
    main_output(MU, "timer 1 interrupt\n");
}
