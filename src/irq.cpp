#include "irq.h"
#include "string.h"

// IRQ handlers, can be set by kernel
irq_handler_t irqh[256];

void irq_init(void)
{
	memset(&irqh, 0, (sizeof(irq_handler_t) * 256));
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
extern "C" void irq_handler(int_reg_t r)
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
