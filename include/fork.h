#ifndef FORK_H
#define FORK_H

#include "types.h"
#include "sched.h"

#define UTHREAD 0
#define KTHREAD 1

int copy_process(u64 clone_flags, u64 fn, u64 arg);
int prepare_move_to_user(u64 start_addr, u64 size, u64 fn);
struct ke_regs * task_ke_regs(struct task_struct *tsk);

/* kernel entry/exit regs */
struct ke_regs 
{
    u64 regs[31];
    u64 sp;
    u64 elr;
    u64 pstate;
};

#endif /* FORK_H */
