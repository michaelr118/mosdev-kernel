#include "idt.h"

#include "irq.h"
#include "string.h"
#include "tty.h"

/*
 * Found in asm/common.s
 * asm_idt_set - Set the Interrupt Descriptor Table (IDT)
 * 32 Required Interrupt Service Routines (ISR) + initial 16 IRQ
*/
void asm_idt_set(uint32_t); void isr0(); void isr1(); void isr2(); void isr3();
void isr4(); void isr5(); void isr6(); void isr7(); void isr8(); void isr9();
void isr10(); void isr11(); void isr12(); void isr13(); void isr14(); void isr15();
void isr16(); void isr17(); void isr18(); void isr19(); void isr20(); void isr21();
void isr22(); void isr23(); void isr24(); void isr25(); void isr26(); void isr27();
void isr28(); void isr29(); void isr30(); void isr31();

void irq0(); void irq1(); void irq2(); void irq3(); void irq4(); void irq5();
void irq6(); void irq7(); void irq8(); void irq9(); void irq10(); void irq11();
void irq12(); void irq13(); void irq14(); void irq15();

uint32_t p_isr[32] = {
(uint32_t)isr0, (uint32_t)isr1, (uint32_t)isr2, (uint32_t)isr3,
(uint32_t)isr4, (uint32_t)isr5, (uint32_t)isr6, (uint32_t)isr7,
(uint32_t)isr8, (uint32_t)isr9, (uint32_t)isr10, (uint32_t)isr11,
(uint32_t)isr12, (uint32_t)isr13, (uint32_t)isr14, (uint32_t)isr15,
(uint32_t)isr16, (uint32_t)isr17, (uint32_t)isr18, (uint32_t)isr19,
(uint32_t)isr20, (uint32_t)isr21, (uint32_t)isr22, (uint32_t)isr23,
(uint32_t)isr24, (uint32_t)isr25, (uint32_t)isr26, (uint32_t)isr27,
(uint32_t)isr28, (uint32_t)isr29, (uint32_t)isr30, (uint32_t)isr31,
};

uint32_t p_irq[16] = {
(uint32_t)irq0, (uint32_t)irq1, (uint32_t)irq2, (uint32_t)irq3,
(uint32_t)irq4, (uint32_t)irq5, (uint32_t)irq6, (uint32_t)irq7,
(uint32_t)irq8, (uint32_t)irq9, (uint32_t)irq10, (uint32_t)irq11,
(uint32_t)irq12, (uint32_t)irq13, (uint32_t)irq14, (uint32_t)irq15,
};
// IDT
idt_entry_t m_IDT[256];
// Pointer to IDT used by _idt_set
idt_ptr_t mp_IDT;

/*
 * Insert entry i into the IDT. Each entry must be formatted correctly.
 * base - start address of function which will handle ISR
 * sel - segment where function resides (0x08, kernel code)
 * opt - options
*/
static void idt_set_entry(uint8_t i, uint32_t base)
{
	m_IDT[i].base_l = (base & 0xffff);
	m_IDT[i].base_high = ((base >> 16) & 0xffff);

	m_IDT[i].sel = 0x08;
	m_IDT[i].rsvd = 0;

	m_IDT[i].opt = 0x8e;
}

/*
 * Installs our IDT (Interrupt Descriptor Table)
 * 0...31 ISR are required by x86 and must be non-null
*/
void idt_init(void)
{
	// Pointer to idt. Limit is always sizeof(idt) - 1
	// base is starting address of the IDT
	mp_IDT.limit = ((sizeof(idt_entry_t) * 256) - 1);
	mp_IDT.base = (uint32_t)(&m_IDT - KERNEL_VIRTUAL_BASE);

	// Zero the idt array initially so we don't get unexpected behavior
	memset(&m_IDT, 0, (sizeof(idt_entry_t) * 256));

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

	for (uint8_t i = 0; i < 48; i++)
	{
		if (i < 32)
			idt_set_entry(i, p_isr[i]);
		else
			idt_set_entry(i, p_irq[-32 + i]);
	}

	// Tells the processor about our new IDT
	asm_idt_set((uint32_t)&mp_IDT);
	irq_init();
	asm volatile("sti");
}
