#include "gpio.h"
#include "peripherals/pl011_uart.h"
#include "pl011_uart.h"

#define TXD4 8
#define RXD4 9

void pl011_uart_init()
{
    /* set gpio 8 and 9 to UART4 */
    gpio_pin_set_func(TXD4, GFAlt4);
    gpio_pin_set_func(RXD4, GFAlt4);
    /* clear the pud resistors (initialize them) */
    gpio_pin_enable(TXD4);
    gpio_pin_enable(RXD4);
    /* 8-bit word size, no parity, yes FIFO, no break */
    REGS_UART4->lcrh = 0x70; //0b0111 0000
    /* interrupt immediately */
    REGS_UART4->ifls = 0;
    /* set baudrate */
    REGS_UART4->ibrd = 26;
    REGS_UART4->fbrd = 3;
    /* only clear RX interuupt mask */
    /* no interrupts now */
    REGS_UART4->imsc = 0x7FF; //0b111 1111 1111
    /* flow control, enable TX RX, enable UART */
    REGS_UART4->cr = 0xC301; //0b1100 0011 0000 0001
}

void pl011_uart_send(char c)
{
    /* keep looping if the 5th bit is 1 */
    while (REGS_UART4->flag & 0x20);

    REGS_UART4->data = c;
}

char pl011_uart_recv()
{
    while (REGS_UART4->flag & 0x10);

    return REGS_UART4->data & 0xFF;
}

void pl011_uart_send_string(char *str)
{
    while (*str) {
        if (*str == '\n')
            /* also do CR if there's a '\n' */
            pl011_uart_send('\r');
        pl011_uart_send(*str);
        str++;
    }
}
