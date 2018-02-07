SC := i686-elf-as
CC := i686-elf-g++

CPPFLAGS := -Isrc/inc -ffreestanding -O2 -Wall -Wextra -nostdlib -fno-exceptions -fno-rtti
LDFLAGS := -Tlinker.ld -ffreestanding -O2 -nostdlib -lgcc

TARGET := vmmos-i386

SRC_DIR := src
OBJ_DIR := obj
OUT_DIR := bin

SYS_ROOT := bin/sysroot

ASM_SRC_FILES := $(wildcard $(SRC_DIR)/asm/*.s)
ASM_OBJ_FILES := $(patsubst $(SRC_DIR)/asm/*.s,$(OBJ_DIR)/%.s.o,$(ASM_SRC_FILES))

CPP_SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
CPP_OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.cpp.o,$(CPP_SRC_FILES))

$(OUT_DIR)/$(TARGET): $(ASM_OBJ_FILES) $(CPP_OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.s.o: $(SRC_DIR)/asm/%.s
	$(SC) $< -o $@

$(OBJ_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<

iso:
	@cp -rfp $(SRC_DIR)/grub.cfg $(SYS_ROOT)/boot/grub/
	@cp -rfp $(OUT_DIR)/$(TARGET) $(SYS_ROOT)/boot/
	@grub-mkrescue -o $(OUT_DIR)/$(TARGET).iso $(SYS_ROOT)
clean:
	@find bin/ -type f |xargs rm -f
	@rm -rf obj/*
