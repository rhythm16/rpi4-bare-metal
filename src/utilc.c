#include "utils.h"
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

void char_to_char_array(char ch, char *buf)
{
    buf[0] = ch;
}

void main_output_char(int interface, char ch)
{
    char printable[2];
    printable[1] = '\0';
    char_to_char_array(ch, printable);
    main_output(interface, printable);
}

void main_output(int interface, char* str)
{
    switch (interface) {
        case (MU):
            mini_uart_send_string(str);
            break;
        default:
            main_output(MU, "main_output bad interface\n");
            break;
    }
}

void main_output_u64(int interface, u64 in)
{
    char printable[17];
    printable[16] = '\0';
    u64_to_char_array(in, printable);
    main_output(interface, printable);
}

void main_output_process(int interface, struct task_struct *p)
{
    main_output(interface, "task address: ");
    main_output_u64(interface, (u64)p);
    main_output(interface, ", state: ");
    main_output_u64(interface, (u64)(p->state));
    main_output(interface, ", counter: ");
    main_output_u64(interface, (u64)(p->counter));
    main_output(interface, ", priority: ");
    main_output_u64(interface, (u64)(p->priority));
    main_output(interface, ", preempt_count: ");
    main_output_u64(interface, (u64)(p->preempt_count));
    main_output(interface, "\n");
}


char main_recv(int interface)
{
    switch (interface) {
        case (MU):
            return mini_uart_recv();
            break;
        default:
            main_output(MU, "main_recv bad interface\n");
            return '\0';
    }
}

void copy_ke_regs(struct ke_regs * to, struct ke_regs * from)
{
    for (int i = 0; i < 31; i++) {
        to->regs[i] = from->regs[i];
    }
    to->sp = from->sp;
    to->elr = from->elr;
    to->pstate = from->pstate;
}

/* bytes is round down to the nearest 8 */
void memcpy(u64* dst, u64* src, u64 bytes)
{
    u64 num = bytes >> 3;
    for (u64 i = 0; i < num; i++) {
        dst[i] = src[i];
    }
}

