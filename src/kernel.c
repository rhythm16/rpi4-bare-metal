/* Makefile's CFLAGS will take care of the include paths */
#include "mini_uart.h"
#include "utils.h"
#include "irq_numbers.h"
#include "irq.h"
#include "generic_timer.h"
#include "fork.h"
#include "sched.h"
#include "mm.h"
#include "sys.h"

#include "trace/trace_main.h"
#include "trace/pl011_uart.h"
#include "trace/ksyms.h"
#include "trace/utils.h"

#include "user_space/init.h"

reg32 state;

/* run by pid 1, will ret back to entry.S and do a kernel_exit 0 */
void kernel_process()
{
    main_output(MU, "pid 1 started in EL");
    main_output_char(MU, get_el() + '0');
    main_output(MU, "\n");

    int err = prepare_move_to_user((u64)user_process);
    if (err < 0)
        main_output(MU, "Failed to move to user mode!\n");
}

void kernel_main(u64 id)
{
    /* core 0 initializes mini-uart and handles uart interrupts */
    if (id == 0) {
        mini_uart_init();
        pl011_uart_init();
        enable_interrupt_gic(VC_AUX_IRQ, id);
        ksyms_init();
        sys_call_table_relocate();
        trace_init();
        trace_output_kernel_pts(PL);
        state = 0;
    }

    /* single core for now */
    while (id != 0) {}

    /* output startup message and EL */
    main_output(MU, "Bare Metal... (core ");
    main_output_char(MU, id + '0');
    main_output(MU, ")\n");
    delay(30000);
    main_output(MU, "EL: ");
    main_output_char(MU, get_el() + '0');
    main_output(MU, "\n");
    /* also output the syscount */
    u64 sys_count = get_sys_count();
    main_output_u64(MU, sys_count);
    main_output(MU, "\n");

    /* initialize exception vectors and timers and the timer interrupt */
    irq_init_vectors();
    generic_timer_init();
    enable_interrupt_gic(NS_PHYS_TIMER_IRQ, id);
    irq_enable();

    /* let the next core run */
    state++;

    while (1) {
        if (id != 0 || state != 1)
            continue;
        sched_init();
        main_output_process(MU, current);
        /* create pid 1, kernel threads don't need a user stack page */
        int res = copy_process(KTHREAD, (u64)&kernel_process, 0, 0);
        if (res <= 0) {
            main_output(MU, "fork error\n");
        }
        while (1) {
            main_output(MU, "init schedule..\n");
            schedule();
        }
    }
}

