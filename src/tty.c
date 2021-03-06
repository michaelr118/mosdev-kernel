#include "common.h"
#include "string.h"
#include "tty.h"

#include "vga.h"

static const size_t TTY_WIDTH = 80;
static const size_t TTY_HEIGHT = 25;

// VGA graphics memory is at memory address 0xb8000
static uint16_t* const VGA_MEMORY = (uint16_t*)(KERNEL_VIRTUAL_BASE + 0xb8000);

static size_t tty_row;
static size_t tty_column;
static uint8_t tty_color;
static uint16_t *tty_buffer;

/*
 * Initialize the terminal to a blank state
*/
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

/*
 * Moves the hardware cursor (blinky underline) to x,y
*/
static void tty_movecursor(size_t x, size_t y)
{
	// Terminal buffer is flat memory, calculate 1-dimensional index from x,y
	const size_t i = ((y * TTY_WIDTH) + x);

	io_outb(0x3d4, 0x0f);
	io_outb(0x3d5, (uint8_t)(i & 0xff));
	io_outb(0x3d4, 0x0e);
	io_outb(0x3d5, (uint8_t)((i >> 8) & 0xff));
}

static void tty_scroll(void)
{
	memcpy(tty_buffer, &tty_buffer[TTY_WIDTH], (TTY_WIDTH * (TTY_HEIGHT - 1)
				* sizeof(uint16_t)));

	for (size_t x = 0; x < TTY_WIDTH; x++)
	{
		const size_t i = (((TTY_HEIGHT - 1) * TTY_WIDTH) + x);
		tty_buffer[i] = vga_entry(' ', tty_color);
	}
}

/*
 * Insert char c with specified color at x,y into the terminal buffer
*/
static void tty_insert(unsigned char c, uint8_t color, size_t x, size_t y)
{
	// Terminal buffer is flat memory, calculate 1-dimensional index from x,y
	const size_t i = ((y * TTY_WIDTH) + x);
	tty_buffer[i] = vga_entry(c, color);
}

/*
 * Prints 1 char to the terminal and increments the position
*/
static void tty_putch(char c)
{
	// Handle newline characters
	if (c == '\n')
	{
		tty_column = 0;
		if (++tty_row == TTY_HEIGHT)
		{
			tty_scroll();
			tty_row--;
		}
		return;
	}

	// Insert char into the terminal buffer
	unsigned char uc = c;
	tty_insert(uc, tty_color, tty_column, tty_row);

	// Bounds check to make sure we're not writing off-screen
	if (++tty_column == TTY_WIDTH)
	{
		tty_column = 0;
		if (++tty_row == TTY_HEIGHT)
		{
			tty_scroll();
			tty_row--;
		}
	}
}

/*
 * Prints a string s of size len to the terminal
*/
void tty_putsl(const char *s, size_t len)
{
	for (size_t i = 0; i < len; i++)
	{
		tty_putch(s[i]);
	}

	// Move the hardware cursor to the current position
	tty_movecursor(tty_column, tty_row);
}

/*
 * Wrapper around tty_puts that handles strlen
*/
void tty_puts(const char *s)
{
	tty_putsl(s, strlen(s));
}

/*
 * Print integer n to the terminal
*/
void tty_putd(uint32_t n)
{
	if (n == 0)
	{
		tty_putch('0');
		return;
	}

	int32_t m = n;
	char c[32];
	int i = 0;

	while (m > 0)
	{
		c[i] = ('0' + (m % 10));
		m /= 10;
		i++;
	}

	c[i] = 0;

	char c2[32];
	c2[i--] = 0;
	int j = 0;

	while (i >= 0)
		c2[i--] = c[j++];

	tty_puts(c2);
}

void tty_puth(uint32_t n)
{
	tty_puts("0x");

	const char hex[16] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
	};

	for (int i = 28; i > 0; i -= 4)
	{
		int hex_i = ((n >> i) & 0xf);
		tty_putsl(&hex[hex_i], 1);
	}
}
