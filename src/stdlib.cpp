#include "stdio.h"
#include "stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
void abort(void)
{
#if defined(__is_libk)
	printf("Kernel panic: abort()\n");
#else
	printf("abort()\n");
#endif
	while (1) { }
	__builtin_unreachable();
}

#ifdef __cplusplus
}
#endif
