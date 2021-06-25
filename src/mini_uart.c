#include "gpio.h"
#include "peripherals/aux.h"
#include "mini_uart.h"

#define TXD 14
#define RXD 15

void uart_init()
{
    /* set pin 14 and 15 to UART1 (mini-uart) */
    gpio_pin_set_func(TXD, GFAlt5);
    gpio_pin_set_func(RXD, GFAlt5);
    /* clear the pud resistors (initialize them) */
    gpio_pin_enable(TXD);
    gpio_pin_enable(RXD);
    /* enable mini-uart */
    REGS_AUX->enables = 1;
    /* disable TX and RX and auto flow control */
    REGS_AUX->mu_control = 0;
    /* disable TX RX interrupts */
    REGS_AUX->mu_ier = 0;
    /* set 8-bit mode */
    REGS_AUX->mu_lcr = 3;
    /* set RTS to always high */
    REGS_AUX->mu_mcr = 0;
    /* 115200 @ 500 MHz */
    REGS_AUX->mu_baud_rate = 541;
    /* enable TX and RX */
    REGS_AUX->mu_control = 3;

    uart_send('\r');
    uart_send('\n');
    uart_send('\n');
}

void uart_send(char c)
{
    /* keep looping if the 5th bit is 0 */
    while (!(REGS_AUX->mu_lsr & 0x20));

    REGS_AUX->mu_io = c;
}

char uart_recv()
{
    while (!(REGS_AUX->mu_lsr & 1));

    return REGS_AUX->mu_io & 0xFF;
}

void uart_send_string(char *str)
{
    while (*str) {
        if (*str == '\n')
            /* also do CR if there's a '\n' */
            uart_send('\r');
        uart_send(*str);
        str++;
    }
}
