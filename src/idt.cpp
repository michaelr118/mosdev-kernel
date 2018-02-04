#include "idt.h"
#include "string.h"
#include "tty.h"

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

void irq0(); void irq1(); void irq2(); void irq3(); void irq4(); void irq5();
void irq6(); void irq7(); void irq8(); void irq9(); void irq10(); void irq11();
void irq12(); void irq13(); void irq14(); void irq15();
}

// IDT
idt_entry_t idt[256];
// Pointer to IDT used by _idt_set
idt_ptr_t idt_ptr;

// IRQ handlers, can be set by kernel
irq_handler_t irqh[256];

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

	// Here we remap the PIC master/slave to use IRQ 32+ since the ISR
	// is required to have 0...31
	io_outb(0x20, 0x11);
	io_outb(0xa0, 0x11);
	
	io_outb(0x21, 0x20);
	io_outb(0xa1, 0x28);
	
	io_outb(0x21, 0x04);
	io_outb(0xa1, 0x02);
	
	io_outb(0x21, 0x01);
	io_outb(0xa1, 0x01);
	
	io_outb(0x21, 0x00);
	io_outb(0xa1, 0x00);

	// Begin mapping ISR 0...31 and IRQ 32+
	
	// ISR
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

	// IRQ
	idt_set_entry(32, (u32int)irq0, 0x08, 0x8e);
	idt_set_entry(33, (u32int)irq1, 0x08, 0x8e);
	idt_set_entry(34, (u32int)irq2, 0x08, 0x8e);
	idt_set_entry(35, (u32int)irq3, 0x08, 0x8e);
	idt_set_entry(36, (u32int)irq4, 0x08, 0x8e);
	idt_set_entry(37, (u32int)irq5, 0x08, 0x8e);
	idt_set_entry(38, (u32int)irq6, 0x08, 0x8e);
	idt_set_entry(39, (u32int)irq7, 0x08, 0x8e);
	idt_set_entry(40, (u32int)irq8, 0x08, 0x8e);
	idt_set_entry(41, (u32int)irq9, 0x08, 0x8e);
	idt_set_entry(42, (u32int)irq10, 0x08, 0x8e);
	idt_set_entry(43, (u32int)irq11, 0x08, 0x8e);
	idt_set_entry(44, (u32int)irq12, 0x08, 0x8e);
	idt_set_entry(45, (u32int)irq13, 0x08, 0x8e);
	idt_set_entry(46, (u32int)irq14, 0x08, 0x8e);
	idt_set_entry(47, (u32int)irq15, 0x08, 0x8e);

	// Tells the processor about our new IDT
	_idt_set((u32int)&idt_ptr);
}

/*
 * Interrupt Service Routines (ISR)
 * Called when the processor has run into a problem that needs to be addressed
 * immediately (such as a page fault, overflow, divide by zero, etc)
*/

/*
 * High-level ISR handler called from isr_common.
 * This is a placeholder for now, but we will need to address these errors eventually
*/
extern "C" void isr_handler(isr_reg_t r)
{
	tty_puts("isr_handler: ");
	tty_putd(r.int_no);
	tty_puts("\n");

	// Since we're not handling the errors, go into an infinite loop
	// Otherwise, we'll just jump back to the problem code and crash the computer
	while (1) { }
}

/*
 * Interrupt Service Requests (IRQ)
 * Called by the PIC when a device needs attention (such as the keyboard, mouse, etc).
 * Not as important as ISR, so our kernel can setup handlers for these as-needed
*/

/*
 * High-level IRQ handler called from irq_common.
 * Informs PIC that the request has been handled then dispatches message
 * to the registered handler (set by irq_set_handler)
*/
extern "C" void irq_handler(isr_reg_t r)
{
	// IRQ was sent by the slave PIC, tell slave PIC we handled the request
	if (r.int_no >= 40)
		io_outb(0xa0, 0x20);

	// Tell master PIC we handled the request
	io_outb(0x20, 0x20);

	// Check if we have a handler for this IRQ. Call that handler
	if (irqh[r.int_no] != 0)
	{
		irq_handler_t handler = irqh[r.int_no];
		handler(r);
	}
}

/*
 * Register a new IRQ handler. Handler will be called whenever IRQ irq_number is
 * received. Passes register state to handler
*/
void irq_set_handler(u8int irq_number, irq_handler_t handler)
{
	irqh[irq_number] = handler;
}
