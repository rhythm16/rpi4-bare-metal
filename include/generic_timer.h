#ifndef GENERIC_TIMER_H
#define GENERIC_TIMER_H

#include "types.h"

#define SYS_FREQ 54000000

u64 get_sys_count();
void set_CNTP_TVAL(u32);
void setup_CNTP_CTL();
void generic_timer_init();
void handle_generic_timer();

#endif /* GENERIC_TIMER_H */
