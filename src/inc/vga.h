#ifndef _KERNEL_VGA_H
#define _KERNEL_VGA_H

#include <stdint.h>

enum VGA_COLOR {
	VGA_COLOR_BLACK,

	VGA_COLOR_BLUE,
	VGA_COLOR_GREEN,
	VGA_COLOR_CYAN,
	VGA_COLOR_RED,
	VGA_COLOR_MAGENTA,
	VGA_COLOR_BROWN,

	VGA_COLOR_LIGHT_GRAY,
	VGA_COLOR_GRAY,

	VGA_COLOR_LIGHT_BLUE,
	VGA_COLOR_LIGHT_GREEN,
	VGA_COLOR_LIGHT_CYAN,
	VGA_COLOR_LIGHT_RED,
	VGA_COLOR_LIGHT_MAGENTA,
	VGA_COLOR_LIGHT_BROWN,

	VGA_COLOR_WHITE
};

static inline uint8_t vga_color(enum VGA_COLOR f, enum VGA_COLOR b)
{
	return (f | b << 4);
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return ((uint16_t)uc | (uint16_t)color << 8);
}

#endif
