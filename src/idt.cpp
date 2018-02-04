#include "idt.h"
#include "string.h"

/*
 * Found in asm/idt.s
 * _idt_set - Set the Interrupt Descriptor Table (IDT)
 * 32 Required Interrupt Service Routines (ISR)
*/
extern "C" { void _idt_set(u32int); void isr0(); void isr1(); void isr2(); void isr3();
void isr4(); void isr5(); void isr6(); void isr7(); void isr8(); void isr9();
void isr10(); void isr11(); void isr12(); void isr13(); void isr14(); void isr15();
void isr16(); void isr17(); void isr18(); void isr19(); void isr20(); void isr21();
void isr22(); void isr23(); void isr24(); void isr25(); void isr26(); void isr27();
void isr28(); void isr29(); void isr30(); void isr31();
}

// IDT
idt_entry_t idt[256];
// Pointer to IDT used by _idt_set
idt_ptr_t idt_ptr;

/*
 * Insert entry i into the IDT. Each entry must be formatted correctly.
 * base - start address of function which will handle ISR
 * sel - segment where function resides (0x08, kernel code)
 * opt - options
*/
static void idt_set_entry(u8int i, u32int base, u16int sel, u8int opt)
{
	idt[i].base_l = (base & 0xffff);
	idt[i].base_high = ((base >> 16) & 0xffff);

	idt[i].sel = sel;
	idt[i].rsvd = 0;

	idt[i].opt = opt;
}

/*
 * Installs our IDT (Interrupt Descriptor Table)
 * 0...31 ISR are required by x86 and must be non-null
*/
void idt_install(void)
{
	// Pointer to idt. Limit is always sizeof(idt) - 1
	// base is starting address of the IDT
	idt_ptr.limit = ((sizeof(idt_entry_t) * 256) - 1);
	idt_ptr.base = (u32int)&idt;

	// Zero the idt array initially so we don't get unexpected behavior
	memset(&idt, 0, (sizeof(idt_entry_t) * 256));

	// 32 Required ISR
	idt_set_entry(0, (u32int)isr0, 0x08, 0x8e);
	idt_set_entry(1, (u32int)isr1, 0x08, 0x8e);
	idt_set_entry(2, (u32int)isr2, 0x08, 0x8e);
	idt_set_entry(3, (u32int)isr3, 0x08, 0x8e);
	idt_set_entry(4, (u32int)isr4, 0x08, 0x8e);
	idt_set_entry(5, (u32int)isr5, 0x08, 0x8e);
	idt_set_entry(6, (u32int)isr6, 0x08, 0x8e);
	idt_set_entry(7, (u32int)isr7, 0x08, 0x8e);
	idt_set_entry(8, (u32int)isr8, 0x08, 0x8e);
	idt_set_entry(9, (u32int)isr9, 0x08, 0x8e);
	idt_set_entry(10, (u32int)isr10, 0x08, 0x8e);
	idt_set_entry(11, (u32int)isr11, 0x08, 0x8e);
	idt_set_entry(12, (u32int)isr12, 0x08, 0x8e);
	idt_set_entry(13, (u32int)isr13, 0x08, 0x8e);
	idt_set_entry(14, (u32int)isr14, 0x08, 0x8e);
	idt_set_entry(15, (u32int)isr15, 0x08, 0x8e);
	idt_set_entry(16, (u32int)isr16, 0x08, 0x8e);
	idt_set_entry(17, (u32int)isr17, 0x08, 0x8e);
	idt_set_entry(18, (u32int)isr18, 0x08, 0x8e);
	idt_set_entry(19, (u32int)isr19, 0x08, 0x8e);
	idt_set_entry(20, (u32int)isr20, 0x08, 0x8e);
	idt_set_entry(21, (u32int)isr21, 0x08, 0x8e);
	idt_set_entry(22, (u32int)isr22, 0x08, 0x8e);
	idt_set_entry(23, (u32int)isr23, 0x08, 0x8e);
	idt_set_entry(24, (u32int)isr24, 0x08, 0x8e);
	idt_set_entry(25, (u32int)isr25, 0x08, 0x8e);
	idt_set_entry(26, (u32int)isr26, 0x08, 0x8e);
	idt_set_entry(27, (u32int)isr27, 0x08, 0x8e);
	idt_set_entry(28, (u32int)isr28, 0x08, 0x8e);
	idt_set_entry(29, (u32int)isr29, 0x08, 0x8e);
	idt_set_entry(30, (u32int)isr30, 0x08, 0x8e);
	idt_set_entry(31, (u32int)isr31, 0x08, 0x8e);

	// Tells the processor about our new IDT
	_idt_set((u32int)&idt_ptr);
}
