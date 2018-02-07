#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include "common.h"

void tty_init(void);
void tty_putchar(char c);
void tty_puts(const char *s, size_t len);
void tty_puts(const char *s);
void tty_putd(u32int n);

#endif
