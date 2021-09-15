#ifndef ENTRY_H
#define ENTRY_H

#define SYNC_INVALID_EL1t       0
#define IRQ_INVALID_EL1t        1
#define FIQ_INVALID_EL1t        2
#define SERROR_INVALID_EL1t     3

#define SYNC_INVALID_EL1h       4
#define IRQ_INVALID_EL1h        5
#define FIQ_INVALID_EL1h        6
#define SERROR_INVALID_EL1h     7

#define SYNC_INVALID_EL0_64     8
#define IRQ_INVALID_EL0_64      9
#define FIQ_INVALID_EL0_64      10
#define SERROR_INVALID_EL0_64   11

#define SYNC_INVALID_EL0_32     12
#define IRQ_INVALID_EL0_32      13
#define FIQ_INVALID_EL0_32      14
#define SERROR_INVALID_EL0_32   15

#define SYNC_ERROR              16
#define SYSCALL_ERROR           17
#define DATA_ABORT_ERROR        18

#define S_FRAME_SIZE            272

#ifndef __ASSEMBLER__

void ret_from_fork();
void err_hang();

#endif /* __ASSEMBLER */
#endif /* ENTRY_H */
