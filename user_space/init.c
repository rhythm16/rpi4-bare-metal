#include "sys.h"
#include "types.h"
#include "user_space/init.h"
#include "utils.h"

void user_process()
{
    call_sys_write("pid 1 in user space\n");
    u64 user_stack_page = call_sys_get_free_page();
    if (user_stack_page < 0) {
        call_sys_write("Error while allocating stack page for pid 2\n");
        goto exit;
    }
    int err = call_sys_clone((u64)user_process1, (u64)"pid2\n", user_stack_page);
    if (err < 0) {
        call_sys_write("Error cloning pid 2\n");
        goto exit;
    }
    user_stack_page = call_sys_get_free_page();
    if (user_stack_page < 0) {
        call_sys_write("Error while allocating stack page for pid 3\n");
        goto exit;
    }
    err = call_sys_clone((u64)user_process1, (u64)"pid3\n", user_stack_page);
    if (err < 0) {
        call_sys_write("Error cloning pid 3\n");
        goto exit;
    }
exit:
    call_sys_exit();
}

void user_process1(char *array)
{
    while (1) {
        call_sys_write(array);
        delay(1000000);
    }
}

