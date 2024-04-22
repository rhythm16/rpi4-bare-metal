#include "types.h"
#include "peripherals/irq.h"
#include "irq_numbers.h"
#include "timer.h"
#include "generic_timer.h"
#include "utils.h"

const char entry_error_messages[19][32] =
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
    "SERROR_INVALID_EL0_32",
    "SYNC_ERROR",
    "SYSCALL_ERROR",
    "DATA_ABORT_ERROR"
};

void show_invalid_entry_message(u32 type, u64 esr, u64 address)
{
    main_output(MU, "ERROR CAUGHT: ");
    main_output(MU, entry_error_messages[type]);
    main_output(MU, ", ESR: ");
    main_output_u64(MU, esr);
    main_output(MU, ", Address: ");
    main_output_u64(MU, address);
    main_output(MU, "\n");
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
            main_output(MU, "Mini-UART Recv: ");
            main_output_char(MU, main_recv(MU));
            main_output(MU, "\n");
            *((reg32*)GICC_EOIR) = IAR;
            break;
        case (NS_PHYS_TIMER_IRQ):
            handle_generic_timer();
            *((reg32*)GICC_EOIR) = IAR;
            if (get_core() == 0) {
                main_output(MU, "core ");
                main_output_char(MU, get_core() + '0');
                main_output(MU, ": generic timer interrupt\n");
                main_output_process(MU, current);
                timer_tick();
            }
            break;
        default:
            main_output(MU, "unknown pending irq\n");
    }
}

