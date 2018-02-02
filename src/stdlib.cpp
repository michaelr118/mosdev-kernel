#include "stdio.h"
#include "stdlib.h"

/*
 * Called when there is a kernel panic
*/
__attribute__((__noreturn__))
void abort(void)
{
	printf("Kernel panic: abort()\n");

	// TODO: Add debug information for kernel panic
	
	while (1) { }
	__builtin_unreachable();
}
