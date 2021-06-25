#ifndef UTILS_H
#define UTILS_H

#include "types.h"

void delay(u64 ticks);
void put32(u64 address, u32 value);
u32 get32(u64 address);

#endif /* UTILS_H */
