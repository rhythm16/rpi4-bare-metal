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
    if (id == 0) {
        uart_init();
        generic_timer_init();
        state = 0;
    }
    while (state != id) {}
    uart_send_string("Bare Metal... (core ");
    uart_send(id + '0');
    uart_send_string(")\n");
    delay(30000);
    uart_send_string("EL: ");
    uart_send(get_el() + '0');
    uart_send_string("\n");
    state++;

    if (id == 0) {
        /* wait for the other cores to finish printing */
        while (state != 4) {}

        irq_init_vectors();
        enable_interrupt_gic(NS_PHYS_TIMER_IRQ, 0);
        enable_interrupt_gic(VC_AUX_IRQ, 0);
        irq_enable();

        while (1) {}
    }
    else {
        while (1) {}
    }
}
