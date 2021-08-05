.globl get_sys_count
get_sys_count:
    mrs x0, CNTPCT_EL0
    ret
