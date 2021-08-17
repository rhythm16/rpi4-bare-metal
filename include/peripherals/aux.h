#ifndef AUX_H
#define AUX_H

#include "types.h"
#include "peripherals/base.h"

/* we only map the part for mini-uart */
struct AuxRegs
{
    reg32 irq_status;
    reg32 enables;
    reg32 reserved[14];
    reg32 mu_io;
    reg32 mu_ier;
    reg32 mu_iir;
    reg32 mu_lcr;
    reg32 mu_mcr;
    reg32 mu_lsr;
    reg32 mu_msr;
    reg32 mu_scratch;
    reg32 mu_control;
    reg32 mu_status;
    reg32 mu_baud_rate;
};

/* 0x00215000 is the offset from PBASE to AuxRegs */
#define REGS_AUX ((struct AuxRegs *)(PBASE + 0x00215000))

#endif /* AUX_H */
