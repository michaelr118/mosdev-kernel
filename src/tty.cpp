#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "ioaccess.inl"
#include "string.h"
#include "tty.h"

#include "vga.h"

static const size_t TTY_WIDTH = 80;
static const size_t TTY_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*)0xb8000;

static size_t tty_row;
static size_t tty_column;
static uint8_t tty_color;
static uint16_t *tty_buffer;

void tty_init(void)
{
	tty_row = 0;
	tty_column = 0;
	tty_color = vga_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK);
	tty_buffer = VGA_MEMORY;

	for (size_t y = 0; y < TTY_HEIGHT; y++)
	{
		for (size_t x = 0; x < TTY_WIDTH; x++)
		{
			const size_t i = ((y * TTY_WIDTH) + x);
			tty_buffer[i] = vga_entry(' ', tty_color);
		}
	}
}

void tty_putentry(unsigned char c, uint8_t color, size_t x, size_t y)
{
	const size_t i = ((y * TTY_WIDTH) + x);
	tty_buffer[i] = vga_entry(c, color);

	io_outb(0x3d4, 0x0f);
	io_outb(0x3d5, (uint8_t)(i & 0xff));
	io_outb(0x3d4, 0x0e);
	io_outb(0x3d5, (uint8_t)((i >> 8) & 0xff));
}

void tty_putch(char c)
{
	unsigned char uc = c;
	tty_putentry(uc, tty_color, tty_column, tty_row);

	if (++tty_column == TTY_WIDTH)
	{
		tty_column = 0;
		if (++tty_row == TTY_HEIGHT)
		{
			tty_row = 0;
		}
	}
}

void tty_put(const char *s, size_t len)
{
	for (size_t i = 0; i < len; i++)
	{
		tty_putch(s[i]);
	}
}

void tty_puts(const char *s)
{
	tty_put(s, strlen(s));
}
