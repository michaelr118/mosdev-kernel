#include "common.h"

#include "gdt.h"
#include "idt.h"
#include "paging.h"
#include "stdio.h"
#include "tty.h"

void kernel_main(void)
{
	tty_init();
	// Global Descriptor Table
	gdt_init();
	// Interrupt Descriptor Table
	idt_init();
	paging_init();

	printf("Welcome.\n\nKernel says...\n\n");

	for (;;)
		asm("hlt");
}
