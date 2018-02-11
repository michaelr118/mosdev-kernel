#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

#include "common.h"

typedef struct GDT_PTR {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

typedef struct GDT_ENTRY {
	uint16_t limit_l;
	uint16_t base_l;
	uint8_t base_middle;
	uint8_t access;
	uint8_t gran;
	uint8_t base_high;
} gdt_entry_t;

void gdt_init(void);

#endif
