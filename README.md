# RPI 4 bare metal

## Errors

This repo contains all kinds of errors, including but not limited to:

- does not boot if you use the ubuntu cross compilation toolchain (I was using [crosstool-ng](https://crosstool-ng.github.io/)'s rpi4 toolchain when developing)
- wrong physical memory layout (I misread `0x3B400000` as `0x38400000`, not breaking, but bad)
- typo(s) in `Documentation.md`
- typo(s) in code comments

Please let me know if you find any other errors, I would like to fix them. At some point...

---

This repo is mostly based on [Low Level Devel's youtube series](https://youtu.be/pd9AVmcRc6U) and [Sergey Matyukevich's tutorial](https://github.com/s-matyukevich/raspberry-pi-os). This version is for __Raspberry pi 4 model b__ only, since it's what I have.

## Current progress

- [x] Lesson 01 (mini-uart output)
- [x] Exercise 1-3 (enable SMP)
- [x] Lesson 02 (sysregs and drop to EL1)
- [x] Exercise 2-2 (enable FP/SIMD registers)
- [x] Lesson 03 (system timer interrupt)
- [x] Exercise 3-2 (Mini-UART recv interrupt)
- [x] Use the GIC-400 as the interrupt controller
- [x] Switch to using the ARM generic timer (with SMP)
- [x] Lesson 04 (kernel threads & the scheduler)
- [x] Exercise 1-2 (pl011-uart)
- [x] Basic Ftrace (works, but lack configurability and slow :) )
- [x] Lesson 05 (user mode and system calls)
- [x] Lesson 06 (virtual memory)

## What you need
* Raspberry pi 4 model b
* USB to TTL serial cable x2
* micro SD card
* SD card reader for your development machine
* terminal emulator which is able to select input source (I use `gtkterm`)

## Deployment
Steps to run this on your RPI4:

1. Download `start4.elf`, `fixup4.dat`, `bcm2711-rpi-4.dtb`, `overlay/miniuart-bt.dtbo` from [RPI's firmware repo](https://github.com/raspberrypi/firmware/tree/master/boot).
2. Put the 4 files in the same directory.
3. Format the micro SD card with the DOS partition table.
4. Create the first partition as FAT32 (`mkfs.vfat`)
5. Modify the first 3 lines of `Makefile`:
```
ARM_GNU: path to your cross compiler, with the prefixes, see mine for example
SD_BOOT: path to the FAT32 partition mounted on your development machine
FIRMWARE: path to the directory containing the 4 files\
They all should not have trailing `/`s.
```
6. Slot in the micro SD card then run `./build.sh`, then umount the card, plug it into the PI4.
7. Connect the USB-TTL cable like so:
![](https://i.imgur.com/8pMcUbv.jpg)
8. Run `gtkterm -p /dev/ttyUSB0 -s 115200` and `gtkterm -p /dev/ttyUSB1 -s 115200` (you might have to add yourself the the `dialout` group).
9. Power on the RPI4, you should see the output like:
```
Bare Metal... (core 0)
EL: 1
0000000032b32b23
task address: ffff000000085800, state: 0000000000000000, counter: 0000000000000000, priority: 0000000000000001, preempt_count: 0000000000000000, pgd: 0000000000000000
created pid 1 at ffff000040000000
init schedule..
pid 1 started in EL1
pid 1 in user space
created pid 2 at ffff000040007000
parent
parent
parent
parent
parent
parent
parent
core 0: generic timer interrupt
task address: ffff000040000000, state: 0000000000000000, counter: 0000000000000001, priority: 0000000000000001, preempt_count: 0000000000000000, pgd: 0000000040002000
child
child
child
child
child
child
child
child
child
...
```
with timer interrupts happening once every second and task switching between init, pid 1 and pid 2.

10. Type some characters, they should be appended:
```
Mini-UART Recv: s
Mini-UART Recv: f
Mini-UART Recv: a
```

11. The other output should contain lots of tracing and debugging info including kernel symbols, ftrace code modification sites, page tables and function traces.

## Caveats & Notes
There are some problems/questions that I found during this:
* Sergey said add `kernel_old=1` in `config.txt`, but it was the opposite, adding the line render it not working (Low level devel doesn't add it either). [This discussion](https://github.com/s-matyukevich/raspberry-pi-os/issues/206) says it is the issue of newer RPI firmware.
* Low level devel said the RPI4's system clock frequency is 500MHz, this is correct in my testing, but I can't find this anywhere, in fact the bcm2711 (the soc on the PI4) peripheral datasheet actually says the system clock frequency is 250MHz on page 10.
* Low level devel uses the RPI3's way to enable the GPIO pins, using the `GPPUD` and `GPPUDCLK` registers, which doesn't even exist on the bcm2711 datasheet, I used the `GPIO_PUP_PDN_CNTRL_REG`s documented in the bcm2711 datasheet. However they both work in my testing.
* In both the bcm2711 and bcm2835's peripheral datasheet, the bits 5:1 of the `AUX_MU_LCR_REG` register is documented as reserved. However `AUX_MU_LCR_REG` must be set to 3 (00b) in order for the miniuart to work properly. This is addressed [here](https://elinux.org/BCM2835_datasheet_errata#p14). I guess the bcm2711 datasheet has the same problem.
* The newer firmware also starts the secondary cpus different from what is said in lesson 01, check the [exercise 1-3 solution for RPI4](https://github.com/s-matyukevich/raspberry-pi-os/blob/master/exercises/lesson01/3/szediwy/src/boot.S) for reference.
* The "functions" in assembly source files do not implement function prologues, be careful not to call functions within a function, otherwise the stack frames would be corrupted.
* For some info on the GIC-400, see what I wrote [here](https://github.com/s-matyukevich/raspberry-pi-os/issues/237).
* The baudrate setting of the PL011 UART was copied from the [exercise solutions](https://github.com/s-matyukevich/raspberry-pi-os/tree/master/exercises/lesson01/2), I don't know how it's calculated and is surprised that the value also works for the RPI4.
* The tutorial assumes the kernel is loaded at physical address 0, so statically initialized pointer values e.g. `current`, `sys_call_table` will be correct, however if this is not the case, a relocation must be performed before using those values.

## Useful Links
[bcm2711 peripherals datasheet](https://datasheets.raspberrypi.org/bcm2711/bcm2711-peripherals.pdf)
