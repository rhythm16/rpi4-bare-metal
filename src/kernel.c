/* Makefile's CFLAGS will take care of the include paths */
#include "mini_uart.h"
#include "pl011_uart.h"
#include "utils.h"
#include "irq_numbers.h"
#include "irq.h"
#include "generic_timer.h"
#include "fork.h"
#include "sched.h"

reg32 state;

void process(char *array)
{
    while (1) {
        mini_uart_send_string(array);
        mini_uart_send_string("\n");
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
    /* core 3 initializes pl011-uart */
    if (id == 3) {
        pl011_uart_init();
        pl011_uart_send_string("pl011-uart initialized\n");
    }

    /* hang then it's not your turn */
    while (state != id) {}

    /* output startup message and EL */
    mini_uart_send_string("Bare Metal... (core ");
    mini_uart_send(id + '0');
    mini_uart_send_string(")\n");
    delay(30000);
    mini_uart_send_string("EL: ");
    mini_uart_send(get_el() + '0');
    mini_uart_send_string("\n");
    /* also output the syscount */
    u64 sys_count = get_sys_count();
    mini_uart_u64(sys_count);
    mini_uart_send_string("\n");

    /* initialize exception vectors and timers and the timer interrupt */
    irq_init_vectors();
    generic_timer_init();
    enable_interrupt_gic(NS_PHYS_TIMER_IRQ, id);
    irq_enable();

    /* let the next core run */
    state++;

    while (1) {
        if (id != 0 || state != 4)
            continue;
        sched_init();
        mini_uart_process(current);
        int res = copy_process((u64)&process, (u64)"task1");
        if (res != 0) {
            mini_uart_send_string("fork error \n");
        }
        res = copy_process((u64)&process, (u64)"task2");
        if (res != 0) {
            mini_uart_send_string("fork error \n");
        }
        while (1) {
            mini_uart_send_string("init schedule..\n");
            schedule();
        }
    }
}
