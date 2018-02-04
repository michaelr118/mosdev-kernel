#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include "common.h"
#define IRQ(x) (x + 32)

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

typedef struct ISR_REG {
	u32int ds;
	u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u32int int_no, err_code;
	u32int eip, cs, opt, uesp, ss;
} isr_reg_t;

typedef void (*irq_handler_t)(isr_reg_t);
void irq_set_handler(u8int irq_number, irq_handler_t handler);

extern "C" void idt_install(void);

#endif
