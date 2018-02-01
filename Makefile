SC := i686-elf-as
CC := i686-elf-g++

CPPFLAGS := -ffreestanding -O2 -Wall -Wextra -nostdlib -fno-exceptions -fno-rtti
LDFLAGS := -Tsrc/linker.ld -ffreestanding -O2 -nostdlib -lgcc

TARGET := vmmos-i386

SRC_DIR := src
OBJ_DIR := obj
OUT_DIR := bin

SYS_ROOT := bin/sysroot

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

$(OUT_DIR)/$(TARGET): $(OBJ_DIR)/boot.o $(OBJ_DIR)/crti.o $(OBJ_DIR)/crtn.o $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<

$(OBJ_DIR)/boot.o: $(SRC_DIR)/boot.s
	$(SC) $< -o $@

$(OBJ_DIR)/crti.o: $(SRC_DIR)/crti.s
	$(SC) $< -o $@

$(OBJ_DIR)/crtn.o: $(SRC_DIR)/crtn.s
	$(SC) $< -o $@

iso:
	@cp -rfp $(SRC_DIR)/grub.cfg $(SYS_ROOT)/boot/grub/
	@cp -rfp $(OUT_DIR)/$(TARGET) $(SYS_ROOT)/boot/
	@grub-mkrescue -o $(OUT_DIR)/$(TARGET).iso $(SYS_ROOT)
clean:
	@find bin/ -type f |xargs rm -f
	@rm -rf obj/*
