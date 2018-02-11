#ifndef _KERNEL_STDIO_H
#define _KERNEL_STDIO_H

#define EOF (-1)

int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);

#endif
