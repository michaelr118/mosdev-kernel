#ifndef _LIBK_STDIO_H
#define _LIBK_STDIO_H

#include "cdefs.h"

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);

#ifdef __cplusplus
}
#endif

#endif