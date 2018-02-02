#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "ioaccess.inl"
#include "stdio.h"
#include "tty.h"

extern "C" void kernel_main(void)
{
	tty_init();
	printf("Welcome. vmmos-i386\n\nKernel says...\n");
}
