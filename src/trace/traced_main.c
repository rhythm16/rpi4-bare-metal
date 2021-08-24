#include "trace/utils.h"
#include "trace/ksyms.h"

void traced(u64 real_func_entry)
{
    char *name = ksym_name_from_addr(real_func_entry - 8);
    /* search success */
    if (name != 0) {
        trace_output(PL, name);
    }
    else {
        trace_output(PL, "NOT FOUND!\n");
    }
    trace_output(PL, "\n");
}

