#ifndef TRACE_UTILS_H
#define TRACE_UTILS_H

#include "types.h"
#include "sched.h"

#define PL 1

void trace_output(int interface, char *str);
void trace_u64_to_char_array(u64 in, char *buf);
void trace_char_to_char_array(char ch, char *buf);
void trace_output_char(int interface, char ch);
void trace_output_u64(int interface, u64 in);
void trace_output_process(int interface, struct task_struct *p);
void trace_output_insn(int interface, u64 addr);
void trace_output_pt(int interface, u64* page);
void trace_output_kernel_pts(int interface);
char trace_recv(int interface);

#endif /* TRACE_UTILS_H */
