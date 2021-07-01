ARM_GNU ?= /home/rhythm/x-tools/aarch64-rpi4-linux-gnu/bin/aarch64-rpi4-linux-gnu
SD_BOOT ?= /media/rhythm/BOOT
FIRMWARE ?= /home/rhythm/project/rpi4-boot

CFLAGS = -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only
ASMFLAGS = -Iinclude

BUILD_DIR = build
SRC_DIR = src

define print_build
	@echo ""
	@echo "================================"
	@echo "Building${BLUE}$(1)${NC} ..."
	@echo "================================"
endef

all : kernel8.img armstub8.bin

clean :
	rm -rf $(BUILD_DIR) *.img *.bin armstub/build

# target in build dir, all files end in _c.o, showing they were created from a .c file
# dependency is  in src, all the .c files
# $<: first dependency
# $@: target
$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c
	$(call print_build, $@)
	mkdir -p $(@D)
	$(ARM_GNU)-gcc $(CFLAGS) -MMD -c $< -o $@

$(BUILD_DIR)/%_S.o: $(SRC_DIR)/%.S
	$(call print_build, $@)
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
	$(call print_build, $@)
	$(ARM_GNU)-ld -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/kernel8.elf $(OBJ_FILES)
	$(ARM_GNU)-objcopy $(BUILD_DIR)/kernel8.elf -O binary $@

armstub/build/armstub8_S.o: armstub/src/armstub8.S
	$(call print_build, $@)
	mkdir -p $(@D)
	$(ARM_GNU)-gcc $(CFLAGS) -MMD -c $< -o $@

armstub8.bin: armstub/build/armstub8_S.o
	$(call print_build, $@)
	$(ARM_GNU)-ld --section-start=.text=0 -o $(<D)/armstub8.elf $<
	$(ARM_GNU)-objcopy $(<D)/armstub8.elf -O binary $@

deploy: kernel8.img config.txt armstub8.bin
	rm -rf $(SD_BOOT)/*
	cp $^ $(SD_BOOT)/
	cp $(FIRMWARE)/bcm2711-rpi-4-b.dtb $(SD_BOOT)/
	cp $(FIRMWARE)/start4.elf $(SD_BOOT)/
	cp $(FIRMWARE)/fixup4.dat $(SD_BOOT)/
	mkdir $(SD_BOOT)/overlays
	cp $(FIRMWARE)/miniuart-bt.dtbo $(SD_BOOT)/overlays
	sync
