ARM_GNU ?= /home/rhythm/x-tools/aarch64-rpi4-linux-gnu/bin/aarch64-rpi4-linux-gnu
SD_BOOT ?= /media/rhythm/BOOT
FIRMWARE ?= /home/rhythm/project/rpi4-boot

CFLAGS = -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only
ASMFLAGS = -Iinclude

BUILD_DIR = build
SRC_DIR = src

all : kernel8.img

clean :
	rm -rf $(BUILD_DIR) *.img

# target in build dir, all files end in _c.o, showing they were created from a .c file
# dependency is  in src, all the .c files
# $<: first dependency
# $@: target
$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(ARM_GNU)-gcc $(CFLAGS) -MMD -c $< -o $@

$(BUILD_DIR)/%_S.o: $(SRC_DIR)/%.S
	mkdir -p $(@D)
	$(ARM_GNU)-gcc $(ASMFLAGS) -MMD -c $< -o $@

C_FILES = $(wildcard $(SRC_DIR)/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/*.S)

# this means to go through ever string in C_FILES and if it matches
# $(SRC_DIR)/%.c, change it to $(BUILD_DIR)/%_c.o
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_S.o)

DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

kernel8.img: $(SRC_DIR)/linker.ld $(OBJ_FILES)
	@echo "Building..."
	@echo ""
	$(ARM_GNU)-ld -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/kernel8.elf $(OBJ_FILES)
	$(ARM_GNU)-objcopy $(BUILD_DIR)/kernel8.elf -O binary kernel8.img

deploy: kernel8.img config.txt
	rm -rf $(SD_BOOT)/*
	cp kernel8.img $(SD_BOOT)/
	cp config.txt $(SD_BOOT)/
	cp $(FIRMWARE)/bcm2711-rpi-4-b.dtb $(SD_BOOT)/
	cp $(FIRMWARE)/start4.elf $(SD_BOOT)/
	cp $(FIRMWARE)/fixup4.dat $(SD_BOOT)/
	mkdir $(SD_BOOT)/overlays
	cp $(FIRMWARE)/miniuart-bt.dtbo $(SD_BOOT)/overlays
	sync
