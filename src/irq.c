#include "irq.h"

#include "intreg.h"
#include "stdio.h"
#include "string.h"

static irq_handler_t m_Handler[256];

void irq_init(void)
{
	memset(&m_Handler, 0, (sizeof(irq_handler_t) * 256));
}

/*
 * Register a new IRQ handler. Handler will be called whenever IRQ irq_number is
 * received. Passes register state to handler
*/
void irq_set_handler(uint8_t irq_number, irq_handler_t handler)
{
	m_Handler[irq_number] = handler;
}

/*
 * Interrupt Service Routines (ISR)
 * Called when the processor has run into a problem that needs to be addressed
 * immediately (such as a page fault, overflow, divide by zero, etc)
*/

/*
 * High-level ISR handler called from isr_common.
*/
void isr_handler(int_reg_t r)
{
	if (m_Handler[r.int_no] != 0)
	{
		irq_handler_t handler = m_Handler[r.int_no];
		handler(r);
	}
	else
	{
		printf("isr: int_no=%u Unhandled ISR. (err_code=%u)\n",
		r.int_no, r.err_code);
		for (;;) { asm volatile("hlt"); }
	}
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
void irq_handler(int_reg_t r)
{
	// IRQ was sent by the slave PIC, tell slave PIC we handled the request
	if (r.int_no >= 40)
		io_outb(0xa0, 0x20);

	// Tell master PIC we handled the request
	io_outb(0x20, 0x20);

	// Check if we have a handler for this IRQ. Call that handler
	if (m_Handler[r.int_no] != 0)
	{
		irq_handler_t handler = m_Handler[r.int_no];
		handler(r);
	}
}
