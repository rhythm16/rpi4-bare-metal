#include "types.h"
#include "mini_uart.h"
#include "peripherals/irq.h"
#include "peripherals/aux.h"

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
    uart_send_string(", ERS: TODO, Address: TODO \n");
}

void enable_core0_interrupt_controller_AUX()
{
    REGS_IRQ0->irq_enable_0 = AUX_IRQ;
}

void handle_irq()
{
    u32 irq;
    irq = REGS_IRQ0->irq_pending_0;
    
    while(irq) {
        if (irq & AUX_IRQ) {
            irq &= ~AUX_IRQ;
            while ((REGS_AUX->mu_iir & 4) == 4) {
                uart_send_string("Mini-UART Recv: ");
                uart_send(uart_recv());
                uart_send_string("\n");
            }
        }
    }
}

