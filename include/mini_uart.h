#ifndef MINI_UART_H
#define MINI_UART_H

void uart_init();
char uart_recv();
void uart_send(char c);
void uart_send_string(char *str);

#endif
