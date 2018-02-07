#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "stdio.h"
#include "tty.h"

extern "C" void kernel_main(void)
{
	tty_init();

	asm volatile ("cli");
	gdt_install();
	idt_install();
	asm volatile ("sti");
	irq_init();

	printf("Welcome. vmmos-i386\n\nKernel says...\n\n");

	for (;;)
		asm("hlt");
}
