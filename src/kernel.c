/* Makefile's CFLAGS will take care of the include paths */
#include "types.h"
#include "mini_uart.h"
#include "utils.h"

reg32 state;

void kernel_main(u64 id)
{
    if (id == 0) {
        uart_init();
        state = 0;
    }
    while (state != id) {}
    uart_send_string("Bare Metal... (core ");
    uart_send(id + '0');
    uart_send_string(")\n");
    delay(30000);
    state++;

    if (id == 0) {
        while (state != 4) {}
        while(1) {
            uart_send(uart_recv());
        }
    }
    else {
        while (1) {}
    }
}
