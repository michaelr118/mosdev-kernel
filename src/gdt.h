#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

#include <stdint.h>

struct GDT_PTR {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

struct GDT_ENTRY {
	unsigned short limit_l;
	unsigned short base_l;
	unsigned char base_middle;
	unsigned char access;
	unsigned char gran;
	unsigned char base_high;
} __attribute__((packed));

void gdt_install(void);

#endif
