#include "trace/utils.h"
#include "types.h"
#include "mm.h"

void do_trace()
{
    while (1) {
        trace_output(PL, "TRACE..\n");
        for (int i = 0; i < 1000000; i++) {
            int k;
            k++;
        }
    }
}

/* relocate .patchable_function_entries section */
void trace_init()
{
    extern u64 __start_patchable_functions[];
    extern u64 __stop_patchable_functions[];
    int count = __stop_patchable_functions - __start_patchable_functions;
    for (int i = 0; i < count; i++) {
        __start_patchable_functions[i] += (u64)KERNEL_START;
    }
    for (int i = 0; i < count; i++) {
        trace_output_u64(PL, __start_patchable_functions[i]);
        trace_output(PL, "\n");
    }
}

