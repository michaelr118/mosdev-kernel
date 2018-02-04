#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "gdt.h"
#include "ioaccess.inl"
#include "stdio.h"
#include "tty.h"

extern "C" void kernel_main(void)
{
	// Install our Global Descriptor Table (GDT)
	// Interrupts should be disabled for this. Re-enabled when done
	__asm__("cli");
	gdt_install();
	__asm__("sti");

	tty_init();
	printf("Welcome. vmmos-i386\n\nKernel says...\n\n");
}
