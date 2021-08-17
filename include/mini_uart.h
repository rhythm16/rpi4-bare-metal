#ifndef MINI_UART_H
#define MINI_UART_H

void mini_uart_init();
char mini_uart_recv();
void mini_uart_send(char c);
void mini_uart_send_string(char *str);

#endif /* MINI_UART_H */
