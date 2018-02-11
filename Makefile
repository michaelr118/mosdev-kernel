SC := nasm
CC := i686-elf-gcc

S_OPT := -felf32
C_OPT := -Isrc/inc -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LD_OPT := -Tlinker.ld -ffreestanding -O2 -nostdlib -lgcc

TARGET := vmmos-i386

SRC_DIR := src
OBJ_DIR := obj
OUT_DIR := bin

SYS_ROOT := bin/sysroot

C_SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
C_OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRC_FILES))

$(OUT_DIR)/$(TARGET): $(OBJ_DIR)/boot.o $(OBJ_DIR)/common.o $(OBJ_DIR)/interr.o $(C_OBJ_FILES)
	$(CC) $(LD_OPT) -o $@ $^

$(OBJ_DIR)/boot.o: $(SRC_DIR)/asm/boot.s
	$(SC) $(S_OPT) $< -o $@

$(OBJ_DIR)/common.o: $(SRC_DIR)/asm/common.s
	$(SC) $(S_OPT) $< -o $@

$(OBJ_DIR)/interr.o: $(SRC_DIR)/asm/interr.s
	$(SC) $(S_OPT) $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(C_OPT) -c -o $@ $<

iso:
	@cp -rfp $(SRC_DIR)/grub.cfg $(SYS_ROOT)/boot/grub/
	@cp -rfp $(OUT_DIR)/$(TARGET) $(SYS_ROOT)/boot/
	@grub-mkrescue -o $(OUT_DIR)/$(TARGET).iso $(SYS_ROOT)
clean:
	@find bin/ -type f |xargs rm -f
	@rm -rf obj/*
