#ifndef TRACE_KSYMS_H
#define TRACE_KSYMS_H

#include "types.h"

extern u64 ksyms;

struct kernel_symbol
{
    u64 address;
    char name[56];
};

char * ksym_name_from_addr(u64 real_func_entry);
void ksyms_init();

#define KERNEL_SYMBOL_TABLE ((struct kernel_symbol *)(&ksyms))

#endif /* TRACE_KSYMS_H */
