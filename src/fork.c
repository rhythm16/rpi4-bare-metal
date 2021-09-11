#include "types.h"
#include "sched.h"
#include "mm.h"
#include "entry.h"
#include "fork.h"
#include "sysregs.h"
#include "utils.h"

int copy_process(u64 clone_flags, u64 fn, u64 arg, u64 user_stack_page)
{
    preempt_disable();
    struct task_struct *p;

    /* allocate page for new task's task_struct, ke_regs and kernel stack */
    p = (struct task_struct *)get_kernel_page();
    if (!p)
        return -1;

    struct ke_regs *childregs = task_ke_regs(p);
    memzero((u64)childregs, sizeof(struct ke_regs));
    memzero((u64)&(p->core_context), sizeof(struct core_context));

    /* set x19 and x20 to run in ret_from_fork if we are creating a kernel thread */
    if (clone_flags & KTHREAD) {
        p->core_context.x19 = fn;
        p->core_context.x20 = arg;
    }
    else {
        struct ke_regs * cur_regs = task_ke_regs(current);
        /* copy the ke_regs from parent, this includes the elr */
        /* special struct copy function is used to avoid gcc generating a memcpy call */
        copy_ke_regs(childregs, cur_regs);
        /* set child's x0 after kernel_exit to 0 to indicate it is child */
        childregs->regs[0] = 0;
        childregs->sp = user_stack_page + PAGE_SIZE;
        /* set task_struct's user_stack_page */
        p->user_stack_page = user_stack_page;
    }

    p->flags = clone_flags;
    p->priority = current->priority;
    p->state = TASK_RUNNING;
    p->counter = p->priority;
    p->preempt_count = 1;

    p->core_context.lr = (u64)ret_from_fork;
    p->core_context.sp = (u64)childregs;

    int pid = nr_tasks++;
    task[pid] = p;
    main_output(MU, "created pid ");
    main_output_char(MU, '0' + pid);
    main_output(MU, " at ");
    main_output_u64(MU, (u64)p);
    main_output(MU, "\n");
    preempt_enable();
    return pid;
}

int prepare_move_to_user(u64 fn)
{
    struct ke_regs *regs = task_ke_regs(current);
    memzero((u64)regs, sizeof(*regs));
    /* jump to fn as EL0 after eret in kernel_exit */
    regs->elr = fn;
    regs->pstate = SPSR_EL1_MODE_EL0t;
    u64 user_stack_page = get_kernel_page();
    if (!user_stack_page)
        return -1;
    regs->sp = user_stack_page + PAGE_SIZE;
    current->user_stack_page = user_stack_page;
    return 0;
}

struct ke_regs * task_ke_regs(struct task_struct *tsk)
{
    u64 p = (u64)tsk + THREAD_SIZE - sizeof(struct ke_regs);
    return (struct ke_regs *)p;
}

