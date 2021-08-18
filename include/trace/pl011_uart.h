#ifndef TRACE_PL011_UART_H
#define TRACE_PL011_UART_H

void pl011_uart_init();
char pl011_uart_recv();
void pl011_uart_send(char c);
void pl011_uart_send_string(char *str);

#endif /* PL011_UART_H */
