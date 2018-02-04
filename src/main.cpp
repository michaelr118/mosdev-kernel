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

	// Dummy interrupt to test IDT
	asm volatile ("int $0x3");
	asm volatile ("int $0x4");
}
