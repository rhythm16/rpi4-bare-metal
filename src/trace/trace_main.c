#include "trace/utils.h"

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

