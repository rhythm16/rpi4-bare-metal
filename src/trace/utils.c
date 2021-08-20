#include "trace/pl011_uart.h"
#include "trace/utils.h"
#include "types.h"
#include "sched.h"

void trace_output(int interface, char *str)
{
    switch(interface) {
        case (PL):
            pl011_uart_send_string(str);
            break;
        default:
            trace_output(PL, "trace_output bad interface\n");
            break;
    }
}

void trace_u64_to_char_array(u64 in, char *buf)
{
    /* every four bits will turn into a printable character */
    for (int i = 0; i < 16; i++) {
        int tmp = (in >> ((15 - i) * 4)) & 0xf;
        if (tmp <= 9) {
            buf[i] = tmp + '0';
        }
        else {
            buf[i] = tmp - 10 + 'a';
        }
    }
}

void trace_char_to_char_array(char ch, char *buf)
{
    buf[0] = ch;
}

void trace_output_char(int interface, char ch)
{
    char printable[2];
    printable[1] = '\0';
    trace_char_to_char_array(ch, printable);
    trace_output(interface, printable);
}

void trace_output_u64(int interface, u64 in)
{
    char printable[17];
    printable[16] = '\0';
    trace_u64_to_char_array(in, printable);
    trace_output(interface, printable);
}

void trace_output_process(int interface, struct task_struct *p)
{
    trace_output(interface, "task address: ");
    trace_output_u64(interface, (u64)p);
    trace_output(interface, ", state: ");
    trace_output_u64(interface, (u64)(p->state));
    trace_output(interface, ", counter: ");
    trace_output_u64(interface, (u64)(p->counter));
    trace_output(interface, ", priority: ");
    trace_output_u64(interface, (u64)(p->priority));
    trace_output(interface, ", preempt_count: ");
    trace_output_u64(interface, (u64)(p->preempt_count));
    trace_output(interface, "\n");
}

void trace_output_insn(int interface, u64 addr)
{
    addr = addr & (~0x7);
    trace_output(interface, "instruction address: ");
    trace_output_u64(interface, addr);
    trace_output(interface, ", instruction: ");
    trace_output_u64(interface, *((u64 *)addr));
    trace_output(interface, "\n");
}

char trace_recv(int interface)
{
    switch (interface) {
        case (PL):
            return pl011_uart_recv();
            break;
        default:
            trace_output(PL, "main_recv bad interface\n");
            return '\0';
    }
}

