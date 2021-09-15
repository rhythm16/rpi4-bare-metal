#ifndef SCHED_H
#define SCHED_H

#define CORE_CONTEXT_OFFSET 0

#ifndef __ASSEMBLER__

#include "types.h"

#define NR_TASKS          64
#define TASK_RUNNING      0
#define TASK_ZOMBIE       1

#define MAX_PAGE_COUNT    16

extern struct task_struct *current;
extern struct task_struct *task[NR_TASKS];
extern int nr_tasks;

struct core_context
{
    u64 x19;
    u64 x20;
    u64 x21;
    u64 x22;
    u64 x23;
    u64 x24;
    u64 x25;
    u64 x26;
    u64 x27;
    u64 x28;
    u64 fp;
    u64 sp;
    u64 lr;
};

struct user_page
{
    u64 pa;
    u64 uva;
};

struct mm_struct
{
    u64 pgd;
    struct user_page user_pages[MAX_PAGE_COUNT];
    u64 kernel_pages[MAX_PAGE_COUNT];
};

struct task_struct
{
    struct core_context core_context;
    long state;
    long counter;
    long priority;
    long preempt_count;
    u64 flags;
    struct mm_struct mm;
};

void core_switch_to(struct task_struct *prev, struct task_struct *next);
void set_pgd(u64 pgd);
void preempt_disable();
void preempt_enable();
void _schedule();
void schedule();
void switch_to(struct task_struct *next);
void timer_tick();
void exit_process();
void sched_init();

#define INIT_TASK \
/* core_context */	{ {0,0,0,0,0,0,0,0,0,0,0,0,0}, \
/* state, counter, priority, preempt_count */	0, 0, 1, 0, \
/* flags */ KTHREAD, \
/* mm */ {0, {{0}}, {0} } \
}

#endif /* __ASSEMBLER__ */
#endif /* SCHED_H */
