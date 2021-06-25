/* Makefile's CFLAGS will take care of the include paths */
#include "types.h"
#include "mini_uart.h"

void kernel_main()
{
    uart_init();
    uart_send_string("Raspberry PI 4 Bare Metal...\n");

    while(1) {
        uart_send(uart_recv());
    }
}
