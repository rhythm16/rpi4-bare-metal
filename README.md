# RPI 4 bare metal

This repo is mostly based on [Low Level Devel's youtube series](https://youtu.be/pd9AVmcRc6U) and [Sergey Matyukevich's tutorial](https://github.com/s-matyukevich/raspberry-pi-os). This version is for __Raspberry pi 4 model b__ only, since it's what I have.

## Current progress

- [x] Lesson 01 (mini-uart output)
- [x] Exercise 1-3 (enable SMP)
- [x] Lesson 02 (sysregs and drop to EL1)
- [x] Exercise 2-2 (enable FP/SIMD registers)
- [x] Mini-UART recv interrupt

## What you need
* Raspberry pi 4 model b
* USB to TTL serial cable
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
6. Slot in the micro SD card then run `make deploy`, then umount the card, plug it into the PI4.
7. Connect the USB-TTL cable like so:
![](https://i.imgur.com/1ohtdeY.jpeg)
8. Run `gtkterm -p /dev/ttyUSB0 -s 115200` (you might have to add yourself the the `dialout` group).
9. Power on the RPI4, you should see the output like:
```
Bare metal... (core 0)
EL: 1
Bare metal... (core 1)
EL: 1
Bare metal... (core 2)
EL: 1
Bare metal... (core 3)
EL: 1
```
10. Type some characters, they should be appended:
```
Mini-UART Recv: s
Mini-UART Recv: f
Mini-UART Recv: a
```
## Caveats
There are some problems/questions that I found during this:
* Sergey said add `kernel_old=1` in `config.txt`, but it was the opposite, adding the line render it not working (Low level devel doesn't add it either). [This discussion](https://github.com/s-matyukevich/raspberry-pi-os/issues/206) says it is the issue of newer RPI firmware.
* Low level devel said the RPI4's system clock frequency is 500MHz, this is correct in my testing, but I can't find this anywhere, in fact the bcm2711 (the soc on the PI4) peripheral datasheet actually says the system clock frequency is 250MHz on page 10.
* Low level devel uses the RPI3's way to enable the GPIO pins, using the `GPPUD` and `GPPUDCLK` registers, which doesn't even exist on the bcm2711 datasheet, I used the `GPIO_PUP_PDN_CNTRL_REG`s documented in the bcm2711 datasheet. However they both work in my testing.
* In both the bcm2711 and bcm2835's peripheral datasheet, the bits 5:1 of the `AUX_MU_LCR_REG` register is documented as reserved. However `AUX_MU_LCR_REG` must be set to 3 (00b) in order for the miniuart to work properly.
* The newer firmware also starts the secondary cpus different from what is said in lesson 01, check the [exercise 1-3 solution for RPI4](https://github.com/s-matyukevich/raspberry-pi-os/blob/master/exercises/lesson01/3/szediwy/src/boot.S) for reference.
* The "functions" in assembly source files do not implement function prologues, be careful not to call functions within a function, otherwise the stack frames would be corrupted.

## Useful Links
[bcm2711 peripherals datasheet](https://datasheets.raspberrypi.org/bcm2711/bcm2711-peripherals.pdf)
