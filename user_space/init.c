#include "sys.h"
#include "types.h"
#include "user_space/init.h"
#include "utils.h"

void loop(char *str)
{
    while (1) {
        call_sys_write(str);
        call_sys_write("\n");
        int i = 100000;
        while (i > 0) {
            i--;
        }
    }
}

void user_process()
{
    call_sys_write("pid 1 in user space\n");
    int pid = call_sys_fork();
    if (pid < 0) {
        call_sys_write("fork error\n");
        call_sys_exit();
        return;
    }
    if (pid == 0) {
        loop("child");
    }
    else {
        loop("parent");
    }
}

