/* Makefile's CFLAGS will take care of the include paths */
#include "types.h"
#include "mini_uart.h"
#include "utils.h"
#include "irq_numbers.h"
#include "irq.h"
#include "timer.h"
#include "generic_timer.h"

reg32 state;

void kernel_main(u64 id)
{
    /* core 0 initializes mini-uart and handles uart interrupts */
    if (id == 0) {
        uart_init();
        enable_interrupt_gic(VC_AUX_IRQ, id);
        state = 0;
    }

    /* hang then it's not your turn */
    while (state != id) {}

    /* output startup message and EL */
    uart_send_string("Bare Metal... (core ");
    uart_send(id + '0');
    uart_send_string(")\n");
    delay(30000);
    uart_send_string("EL: ");
    uart_send(get_el() + '0');
    uart_send_string("\n");
    /* also output the syscount */
    u64 sys_count = get_sys_count();
    char printable[17];
    u64_to_char_array(sys_count, printable);
    printable[16] = '\0';
    uart_send_string(printable);
    uart_send_string("\n");

    /* initialize exception vectors and timers and the timer interrupt */
    irq_init_vectors();
    generic_timer_init();
    enable_interrupt_gic(NS_PHYS_TIMER_IRQ, id);
    irq_enable();

    /* let the next core run */
    state++;

    while (1) {}
}
