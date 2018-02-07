#include "idt.h"
#include "string.h"
#include "tty.h"

/*
 * Found in asm/idt.s
 * _idt_set - Set the Interrupt Descriptor Table (IDT)
 * 32 Required Interrupt Service Routines (ISR) + initial 16 IRQ
*/
extern "C" { void _idt_set(u32int); void isr0(); void isr1(); void isr2(); void isr3();
void isr4(); void isr5(); void isr6(); void isr7(); void isr8(); void isr9();
void isr10(); void isr11(); void isr12(); void isr13(); void isr14(); void isr15();
void isr16(); void isr17(); void isr18(); void isr19(); void isr20(); void isr21();
void isr22(); void isr23(); void isr24(); void isr25(); void isr26(); void isr27();
void isr28(); void isr29(); void isr30(); void isr31();

void irq0(); void irq1(); void irq2(); void irq3(); void irq4(); void irq5();
void irq6(); void irq7(); void irq8(); void irq9(); void irq10(); void irq11();
void irq12(); void irq13(); void irq14(); void irq15();
}

u32int p_isr[32] = {
(u32int)isr0, (u32int)isr1, (u32int)isr2, (u32int)isr3,
(u32int)isr4, (u32int)isr5, (u32int)isr6, (u32int)isr7,
(u32int)isr8, (u32int)isr9, (u32int)isr10, (u32int)isr11,
(u32int)isr12, (u32int)isr13, (u32int)isr14, (u32int)isr15,
(u32int)isr16, (u32int)isr17, (u32int)isr18, (u32int)isr19,
(u32int)isr20, (u32int)isr21, (u32int)isr22, (u32int)isr23,
(u32int)isr24, (u32int)isr25, (u32int)isr26, (u32int)isr27,
(u32int)isr28, (u32int)isr29, (u32int)isr30, (u32int)isr31,
};

u32int p_irq[16] = {
(u32int)irq0, (u32int)irq1, (u32int)irq2, (u32int)irq3,
(u32int)irq4, (u32int)irq5, (u32int)irq6, (u32int)irq7,
(u32int)irq8, (u32int)irq9, (u32int)irq10, (u32int)irq11,
(u32int)irq12, (u32int)irq13, (u32int)irq14, (u32int)irq15,
};

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
static void idt_set_entry(u8int i, u32int base)
{
	idt[i].base_l = (base & 0xffff);
	idt[i].base_high = ((base >> 16) & 0xffff);

	idt[i].sel = 0x08;
	idt[i].rsvd = 0;

	idt[i].opt = 0x8e;
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

	// Here we remap the PIC master/slave to use IRQ 32+ since the ISR
	// is required to have 0...31
	
	// Save master/slave state
	unsigned char m, s;
	m = io_inb(0x20);
	s = io_inb(0xa0);

	// Begin remap
	io_outb(0x20, 0x11);
	io_outb(0xa0, 0x11);

	// Master offset 32
	io_outb(0x21, 0x20);
	// Slave offset 40
	io_outb(0xa1, 0x28);

	// Tell master about slave
	io_outb(0x21, 0x04);
	// Tell slave about master
	io_outb(0xa1, 0x02);

	// Put both master/slave in 8086 mode (0x01)
	io_outb(0x21, 0x01);
	io_outb(0xa1, 0x01);

	// Restore saved state
	io_outb(0x21, m);
	io_outb(0xa1, s);

	// Temporary: tells PIC that we only care about keyboard interrupts
	io_outb(0x21, 0xfd);
	io_outb(0xa1, 0xff);

	for (u8int i = 0; i < 48; i++)
	{
		if (i < 32)
			idt_set_entry(i, p_isr[i]);
		else
			idt_set_entry(i, p_irq[i-32]);
	}

	// Tells the processor about our new IDT
	_idt_set((u32int)&idt_ptr);
}
