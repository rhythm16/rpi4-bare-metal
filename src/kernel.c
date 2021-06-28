/* Makefile's CFLAGS will take care of the include paths */
#include "types.h"
#include "mini_uart.h"
#include "utils.h"

reg32 state;

void kernel_main()
{
    uart_init();
    state = 0;
    uart_send_string("Bare Metal... (core 0)\n");
    delay(30000);
    state = 1;

    while (state != 4) {}
    while(1) {
        uart_send(uart_recv());
    }
}

void kernel_app(u64 id)
{
    while (state != id) {
        delay(100);
    }
    uart_send_string("Bare Metal... (core ");
    uart_send(id + '0');
    uart_send_string(")\n");
    delay(30000);
    state++;

    while(1) {}
}
