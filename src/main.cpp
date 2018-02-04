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
	gdt_install();

	tty_init();
	printf("Welcome. vmmos-i386\n\nKernel says...\n\n");
}
