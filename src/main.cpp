#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "stdio.h"
#include "tty.h"

extern "C" void kernel_main(void)
{
	tty_init();
	printf("Welcome. vmmos-i386\n\nKernel says...\n\n");

	asm volatile ("int $0x3");
	asm volatile ("int $0x4");
}
