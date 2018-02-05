#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "gdt.h"
#include "idt.h"
#include "stdio.h"
#include "tty.h"

extern "C" void kernel_main(void)
{
	asm volatile ("cli");
	gdt_install();
	idt_install();
	asm volatile ("sti");

	tty_init();
	printf("Welcome. vmmos-i386\n\nKernel says...\n\n");

	asm volatile ("int $0x3");

	for (;;)
		asm("hlt");
}
