/* Makefile's CFLAGS will take care of the include paths */
#include "mini_uart.h"
#include "utils.h"
#include "irq_numbers.h"
#include "irq.h"
#include "generic_timer.h"
#include "fork.h"
#include "sched.h"
#include "mm.h"

#include "trace/trace_main.h"
#include "trace/pl011_uart.h"

reg32 state;

void process(char *array)
{
    while (1) {
        main_output(MU, array);
        main_output(MU, "\n");
        delay(1000000);
    }
}

void kernel_main(u64 id)
{
    /* core 0 initializes mini-uart and handles uart interrupts */
    if (id == 0) {
        mini_uart_init();
        enable_interrupt_gic(VC_AUX_IRQ, id);
        state = 0;
    }
    /* core 3 initializes pl011-uart and goes its own ways, without even setting up the interrupt table */
    if (id == 3) {
        pl011_uart_init();
        pl011_uart_send_string("pl011-uart initialized\n");
        /* this does not return */
        do_trace();
    }

    /* hang then it's not your turn */
    while (state != id) {}

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
        if (id != 0 || state != 3)
            continue;
        sched_init();
        main_output_process(MU, current);
        int res = copy_process((u64)&process, (u64)"task1");
        if (res != 0) {
            main_output(MU, "fork error \n");
        }
        res = copy_process((u64)&process, (u64)"task2");
        if (res != 0) {
            main_output(MU, "fork error \n");
        }
        while (1) {
            main_output(MU, "init schedule..\n");
            schedule();
        }
    }
}
