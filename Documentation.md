# Documentation

## Memory Management
* 4K pages are used
* Cores' boot time stack pointer is placed at 4M, 6M, 8M, 10M

### RPi4 Physical Memory Layout
0x40000000 - 0xFC000000 (1G ~ 3G960M) free

0x38400000 - 0x40000000 (948M ~ 1G) VC used

0x00000000 - 0x38400000 (0 ~ 948M) free

### Physical Memory Dynamic Allocation
`get_free_page()` allocates physical frames from 1G upwards to 3G960M

### 

## User space
Because we don't have storage support, the only way for user programs to work is to link it inside the kernel image, but this also causes the ksyms to include symbols in user space. Later on it might be possible to compile kernel and user code separately, stick them together raw, and load it as the "kernel image".

## Tracing

## Ksyms
Short for kernel symbols, used by tracing, main purpose is to be able to translate addresses to symbol names in runtime.

### Ksyms notes
* `_symbols`: section name for the symbol area
* `src/symbol_area.S`: assembly file generated after the first pass, containing formated `nm` output, to be linked in in the second pass
* `ksyms`: the symbol which code accesses, it is the start of the symbol area

### Ksyms implementation

## Concerns (nasty stuff)
* `syms_values.symbol_area_file` has to be under the `src/` directory
* Is `bss_end` section close to the limit of the `adr` instruction at the start of the booting process?
* There are two copies of output formating code for MiniUART and PL011UART

## Tools used on development machine
* `python3`, for symbol generation
* `nm` (both native and cross compiling version should work)
