#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include "common.h"

typedef struct IDT_ENTRY {
	uint16_t base_l;
	uint16_t sel;
	uint8_t rsvd;
	uint8_t opt;
	uint16_t base_high;
} idt_entry_t;

typedef struct IDT_PTR {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idt_ptr_t;

void idt_init(void);

#endif
