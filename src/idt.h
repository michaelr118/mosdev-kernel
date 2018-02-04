#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include "common.h"

typedef struct IDT_ENTRY {
	u16int base_l;
	u16int sel;
	u8int rsvd;
	u8int opt;
	u16int base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct IDT_PTR {
	u16int limit;
	u32int base;
} __attribute__((packed)) idt_ptr_t;

#endif
