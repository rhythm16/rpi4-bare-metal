#ifndef PER_GPIO_H
#define PER_GPIO_H

#include "types.h"
#include "peripherals/base.h"

struct GpioPinData {
    reg32 reserved;
    reg32 data[2];
};

struct GpioRegs {
    reg32 func_select[6];
    struct GpioPinData output_set;
    struct GpioPinData output_clear;
    struct GpioPinData level;
    struct GpioPinData ev_detect_status;
    struct GpioPinData re_detect_enable;
    struct GpioPinData fe_detect_enable;
    struct GpioPinData hi_detect_enable;
    struct GpioPinData lo_detect_enable;
    struct GpioPinData async_re_detect;
    struct GpioPinData async_fe_detect;
    reg32 reserved;
    reg32 pupd_enable;
    reg32 pupd_enable_clocks[2];
    reg32 reserved2[18];
    reg32 pullup_pulldown[4];
};

/* GpioRegs start at 0xfe200000 */
#define REGS_GPIO ((struct GpioRegs *)(PBASE + 0x00200000))

#endif /* PER_GPIO_H */
