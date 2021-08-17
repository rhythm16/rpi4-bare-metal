#ifndef PER_PL011_UART_H
#define PER_PL011_UART_H

#include "types.h"
#include "peripherals/base.h"

struct pl011_uart_regs
{
    reg32 data;
    reg32 rsrecr;
    reg32 reserved[4];
    reg32 flag;
    reg32 reserved_1;
    reg32 ilpr;
    reg32 ibrd;
    reg32 fbrd;
    reg32 lcrh;
    reg32 cr;
    reg32 ifls;
    reg32 imsc;
    reg32 ris;
    reg32 mis;
    reg32 icr;
    reg32 dmacr;
    reg32 reserved_2[13];
    reg32 itcr;
    reg32 itip;
    reg32 itop;
    reg32 tdr;
};

#define REGS_UART4 ((struct pl011_uart_regs *)(PBASE + 0x201800))

#endif /* PER_PL011_UART_H */
