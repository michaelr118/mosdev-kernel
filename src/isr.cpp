#include "isr.h"
#include "tty.h"

/*
 * High-level ISR handler called from isr_common
*/
extern "C" void isr_handler(isr_reg_t r)
{
	tty_puts("isr_handler: ");
	tty_putd(r.int_no);
	tty_puts("\n");

	while (1) { }
}
