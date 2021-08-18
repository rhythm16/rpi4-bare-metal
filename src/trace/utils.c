#include "pl011_uart.h"
#include "trace/utils.h"

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
