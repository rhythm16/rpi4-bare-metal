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

void mini_uart_u64(u64 in)
{
    char printable[17];
    printable[16] = '\0';
    u64_to_char_array(in, printable);
    mini_uart_send_string(printable);
}

void mini_uart_process(struct task_struct *p)
{
    mini_uart_send_string("task address: ");
    mini_uart_u64((u64)p);
    mini_uart_send_string(", state: ");
    mini_uart_u64((u64)(p->state));
    mini_uart_send_string(", counter: ");
    mini_uart_u64((u64)(p->counter));
    mini_uart_send_string(", priority: ");
    mini_uart_u64((u64)(p->priority));
    mini_uart_send_string(", preempt_count: ");
    mini_uart_u64((u64)(p->preempt_count));
    mini_uart_send_string("\n");
}
