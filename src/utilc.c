#include "types.h"
#include "mini_uart.h"
#include "sched.h"

void u64_to_char_array(u64 in, char *buf)
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

void uart_u64(u64 in)
{
    char printable[17];
    printable[16] = '\0';
    u64_to_char_array(in, printable);
    uart_send_string(printable);
}

void uart_process(struct task_struct *p)
{
    uart_send_string("task address: ");
    uart_u64((u64)p);
    uart_send_string(", state: ");
    uart_u64((u64)(p->state));
    uart_send_string(", counter: ");
    uart_u64((u64)(p->counter));
    uart_send_string(", priority: ");
    uart_u64((u64)(p->priority));
    uart_send_string(", preempt_count: ");
    uart_u64((u64)(p->preempt_count));
    uart_send_string("\n");
}
