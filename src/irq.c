#include "types.h"
#include "mini_uart.h"
#include "peripherals/irq.h"
#include "irq_numbers.h"
#include "peripherals/aux.h"
#include "timer.h"
#include "generic_timer.h"
#include "utils.h"

const char entry_error_messages[16][32] = 
{
    "SYNC_INVALID_EL1t", 
    "IRQ_INVALID_EL1t",
    "FIQ_INVALID_EL1t",
    "SERROR_INVALID_EL1t",
    "SYNC_INVALID_EL1h", 
    "IRQ_INVALID_EL1h",
    "FIQ_INVALID_EL1h",
    "SERROR_INVALID_EL1h",
    "SYNC_INVALID_EL0_64", 
    "IRQ_INVALID_EL0_64",
    "FIQ_INVALID_EL0_64",
    "SERROR_INVALID_EL0_64",
    "SYNC_INVALID_EL0_32", 
    "IRQ_INVALID_EL0_32",
    "FIQ_INVALID_EL0_32",
    "SERROR_INVALID_EL0_32"
};

void show_invalid_entry_message(u32 type, u64 esr, u64 address) 
{
    uart_send_string("ERROR CAUGHT: ");
    uart_send_string(entry_error_messages[type]);
    uart_send_string(", ESR: TODO, Address: TODO \n");
}

void enable_gic_distributor(u32 INTID)
{
    u32 n = INTID / 32;
    u32 shift = INTID % 32;
    GICD_ISENABLERN->bitmap[n] |= (1 << shift);
}

void assign_interrupt_core(u32 INTID, u32 core)
{
    u32 n = INTID / 4;
    u32 byte_offset = INTID % 4;
    u32 shift = byte_offset * 8 + core;
    GICD_ITARGETSRN->set[n] |= (1 << shift);
}

void enable_interrupt_gic(u32 INTID, u32 core)
{
    enable_gic_distributor(INTID);
    assign_interrupt_core(INTID, core);
}

void handle_irq()
{
    reg32 IAR = *((reg32*)GICC_IAR);
    u32 INTID = IAR & 0x2FF;
    switch (INTID) {
        case (VC_TIMER_IRQ_1):
            handle_sys_timer_1();
            *((reg32*)GICC_EOIR) = IAR;
            break;
        case (VC_AUX_IRQ):
            uart_send_string("Mini-UART Recv: ");
            uart_send(uart_recv());
            uart_send_string("\n");
            *((reg32*)GICC_EOIR) = IAR;
            break;
        case (NS_PHYS_TIMER_IRQ):
            if (get_core() == 0)
                uart_send_string("reset timer\n");
            handle_generic_timer();
            if (get_core() == 0)
                uart_send_string("write back IAR\n");
            *((reg32*)GICC_EOIR) = IAR;
            if (get_core() == 0) {
                uart_send_string("core ");
                uart_send(get_core() + '0');
                uart_send_string(": generic timer interrupt\n");
                uart_process(current);
                timer_tick();
            }
            break;
        default:
            uart_send_string("unknown pending irq\n");
    }
}
