#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

#include <stdint.h>
#include "common.h"

typedef struct GDT_PTR {
	u16int limit;
	u32int base;
} __attribute__((packed)) gdt_ptr_t;

typedef struct GDT_ENTRY {
	u16int limit_l;
	u16int base_l;
	u8int base_middle;
	u8int access;
	u8int gran;
	u8int base_high;
} __attribute__((packed)) gdt_entry_t;

extern "C" void gdt_install(void);

#endif
