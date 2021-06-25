#include "gpio.h"
#include "utils.h"

void gpio_pin_set_func(u8 pinNumber, GpioFunc func)
{
    u8 bit_start = (pinNumber % 10) * 3;
    u8 reg = pinNumber / 10;
    u32 selector = REGS_GPIO->func_select[reg];
    /* clear out the 3 bits we want to modify */
    selector &= ~(7 << bit_start);
    /* then or it in */
    selector |= (func << bit_start);
    REGS_GPIO->func_select[reg] = selector;
}

/*
 * use neither pull up or pull down registers,
 * because we connect them all the time using the mini-uart.
 */
 void gpio_pin_enable(u8 pinNumber)
{
    reg32 reg = REGS_GPIO->pullup_pulldown[pinNumber/16];
    u8 bit_start = (pinNumber % 16) * 2;
    reg &= ~(3 << bit_start);
    REGS_GPIO->pullup_pulldown[pinNumber/16] = reg;
}
