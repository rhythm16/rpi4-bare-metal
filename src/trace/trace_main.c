#include "trace/utils.h"
#include "trace/hook.h"
#include "types.h"
#include "mm.h"
#include "trace/insn.h"

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

/* ideally, send IPIs and let all cores spin on some stuff */
void gather_cores()
{
}

void put_back_cores()
{
}

/* return how many instructions between addr and the hook */
int trace_calculate_offset(u64 addr)
{
    /* we want signed values, pray to not exceed 2^26 instructions */
    return ((int64_t)(&hook) - (int64_t)(addr)) / 4;
}

/* offset is the instruction count */
u32 trace_generate_bl(int offset)
{
    /* bl is 0b1001 01xx xxxx xxxx xxxx xxxx xxxx xxxx */
    u64 bl = BL_OP;
    offset = offset & BL_MASK;
    trace_output(PL, "generated: ");
    trace_output_u64(PL, (u64)(bl | offset));
    trace_output(PL, "\n");
    return bl | offset;
}

void trace_modify_code(u64 addr, u32 insn)
{
    *((u32 *)addr) = insn;
}

void trace_relocate(u64 *start, u64 *end)
{
    int count = end - start;
    for (int i = 0; i < count; i++) {
        start[i] += (u64)KERNEL_START;
    }
}

void trace_setup_movx9lr(u64 *start, u64 *end)
{
    int count = end - start;
    for (int i = 0; i < count; i++) {
        u32 insn = MOV_X9_LR;
        trace_modify_code(start[i], insn);
        trace_output_insn(PL, start[i]);
    }
}

void trace_enable(u64 *start, u64 *end)
{
    int count = end - start;
    gather_cores();
    /* calculate bl offset and modify code */
    for (int i = 0; i < count; i++) {
        int offset = trace_calculate_offset(start[i] + 4);
        u32 insn = trace_generate_bl(offset);
        trace_modify_code(start[i] + 4, insn);
        trace_output_insn(PL, start[i] + 4);
    }
    put_back_cores();
}

void trace_init()
{
    extern u64 __start_patchable_functions[];
    extern u64 __stop_patchable_functions[];
    trace_relocate(__start_patchable_functions, __stop_patchable_functions);
    trace_setup_movx9lr(__start_patchable_functions, __stop_patchable_functions);
    trace_output(PL, "modified mov x9, lr\n");
    trace_enable(__start_patchable_functions, __stop_patchable_functions);
}

