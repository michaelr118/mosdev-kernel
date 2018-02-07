#include "intreg.h"
#include "tty.h"

/*
 * Interrupt Service Routines (ISR)
 * Called when the processor has run into a problem that needs to be addressed
 * immediately (such as a page fault, overflow, divide by zero, etc)
*/

/*
 * High-level ISR handler called from isr_common.
 * This is a placeholder for now, but we will need to address these errors eventually
*/
extern "C" void isr_handler(int_reg_t r)
{
	tty_puts("isr: int_no=");
	tty_putd(r.int_no);
	tty_puts(" ");

	switch (r.int_no)
	{
		case 0:
			tty_puts("Division by zero");
			break;
		case 1:
			tty_puts("Single-step interrupt");
			break;
		case 2:
			tty_puts("NMI");
			break;
		case 3:
			tty_puts("Breakpoint triggered");
			break;
		case 4:
			tty_puts("Overflow detected");
			break;
		case 5:
			tty_puts("Bounds error");
			break;
		case 6:
			tty_puts("Invalid opcode");
			break;
		case 7:
			tty_puts("Coprocessor not available");
			break;
		case 8:
			tty_puts("Double fault");
			break;
		case 9:
			tty_puts("Coprocessor segment overrun");
			break;
		case 10:
			tty_puts("Invalid TSS");
			break;
		case 11:
			tty_puts("Segment not present");
			break;
		case 12:
			tty_puts("Stack fault");
			break;
		case 13:
			tty_puts("General protection fault");
			break;
		case 14:
			tty_puts("Page fault");
			break;
		case 16:
			tty_puts("Math fault");
			break;
		case 17:
			tty_puts("Alignment check");
			break;
		case 18:
			tty_puts("Machine check");
			break;
		case 19:
			tty_puts("Floating-point exception");
			break;
		case 20:
			tty_puts("Virtualization exception");
			break;
		case 21:
			tty_puts("Control protection exception");
			break;
		default:
			tty_puts("General exception");
			break;
	}

	tty_puts(" (err_code=");
	tty_putd(r.err_code);
	tty_puts(").\n");

	// Temporary, we will need to address these errors eventually
	while (1) { }
}
