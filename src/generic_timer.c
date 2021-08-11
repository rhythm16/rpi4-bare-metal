#include "mini_uart.h"
#include "generic_timer.h"
#include "utils.h"

void generic_timer_init()
{
    setup_CNTP_CTL();
    set_CNTP_TVAL(SYS_FREQ);
}

void handle_generic_timer()
{
    /*
    uart_send_string("core ");
    uart_send(get_core() + '0');
    uart_send_string(": generic timer interrupt\n");
    */
    set_CNTP_TVAL(SYS_FREQ);
}

