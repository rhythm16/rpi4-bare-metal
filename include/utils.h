#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include "sched.h"

/* in utils.S */
void delay(u64 ticks);
void put32(u64 address, u32 value);
u32 get32(u64 address);
u32 get_el();
u32 get_core();

/* in utilc.c */
void u64_to_char_array(u64 in, char *buf);
void uart_u64(u64 in);
void uart_process(struct task_struct *p);

#endif /* UTILS_H */
