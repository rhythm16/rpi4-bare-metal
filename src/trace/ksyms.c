#include "trace/ksyms.h"
#include "trace/utils.h"
#include "mm.h"

u64 ksyms_count = 0;

/* linear search for now */
char * ksym_name_from_addr(u64 addr)
{
    struct kernel_symbol * ks_ptr = KERNEL_SYMBOL_TABLE;
    for (int i = 0; i < ksyms_count; i++) {
        if (ks_ptr[i].address == addr) {
            return ks_ptr[i].name;
        }
    }
    return 0;
}

void relocate_ksyms()
{
    struct kernel_symbol * ks_ptr = KERNEL_SYMBOL_TABLE;
    for (int i = 0; i < ksyms_count; i++) {
        ks_ptr[i].address += (u64)KERNEL_START;
    }
}

void cal_ksyms_count()
{
    struct kernel_symbol * ks_ptr = KERNEL_SYMBOL_TABLE;

    int i = 0;
    u64 count = 0;
    
    while ((ks_ptr[i].name)[0] != 0) {
        i++;
        count++;
    }
    ksyms_count = count;
}

void ksyms_init()
{
    struct kernel_symbol * ks_ptr = KERNEL_SYMBOL_TABLE;

    cal_ksyms_count();
    trace_output(PL, "found ");
    trace_output_u64(PL, ksyms_count);
    trace_output(PL, " kernel symbols\n");
    relocate_ksyms();

    for (int i = 0; i < ksyms_count; i++) {
        trace_output_u64(PL, ks_ptr[i].address);
        trace_output(PL, " ");
        trace_output(PL, ks_ptr[i].name);
        trace_output(PL, "\n");
    }
}

