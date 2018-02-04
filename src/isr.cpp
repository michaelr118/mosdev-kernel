#include "isr.h"
#include "stdio.h"

extern "C" void isr_handler(isr_reg_t r)
{
	printf("Received interrupt.\n");

	while (1) { }
}
