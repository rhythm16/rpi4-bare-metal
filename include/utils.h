#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include "sched.h"
#include "fork.h"

#define MU 0

/* in utils.S */
void delay(u64 ticks);
void put32(u64 address, u32 value);
u32 get32(u64 address);
u32 get_el();
u32 get_core();

/* in utilc.c */
void u64_to_char_array(u64 in, char *buf);
void char_to_char_array(char ch, char *buf);
void main_output_process(int interface, struct task_struct *p);
void main_output_u64(int interface, u64 in);
void main_output(int interface, char* str);
void main_output_char(int interface, char ch);
char main_recv(int interface);

void copy_ke_regs(struct ke_regs * to, struct ke_regs * from);
void memcpy(u64* dst, u64* src, u64 bytes);
void panic(char *msg);

#endif /* UTILS_H */
