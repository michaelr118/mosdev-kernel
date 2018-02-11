#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>

#include "stdio.h"
#include "string.h"
#include "tty.h"

static bool print(const char *s, size_t len)
{
	const unsigned char *c = (const unsigned char*)s;
	for (size_t i = 0; i < len; i++)
		if (putchar(c[i]) == EOF)
			return false;
	return true;

}

int printf(const char* __restrict format, ...)
{
	va_list param;
	va_start(param, format);

	int n = 0;

	while (*format != '\0')
	{
		size_t maxrem = (INT_MAX - n);

		if (format[0] != '%' || format[1] == '%')
		{
			if (format[0] == '%')
				format++;

			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;

			if (maxrem < amount)
				return -1;

			if (!print(format, amount))
				return -1;

			format += amount;
			n += amount;

			continue;
		}

		const char *format_begin = format++;

		if (*format == 'c')
		{
			format++;
			char c = (char)va_arg(param, int);
			if (!maxrem)
				return -1;
			if (!print(&c, sizeof(c)))
				return -1;
			n++;
		}
		else if (*format == 's')
		{
			format++;
			const char *str = va_arg(param, const char*);
			size_t len = strlen(str);
			if (maxrem < len)
				return -1;
			if (!print(str, len))
				return -1;
			n += len;
		}
		else if (*format == 'u')
		{
			format++;
			uint32_t d = va_arg(param, uint32_t);
			uint32_t tmp = d;
			size_t len = 0;
			do { tmp /= 10; len++; } while (tmp != 0);
			if (maxrem < len)
				return -1;
			tty_putd(d);
			n += len;
		}
		else if (*format == 'x')
		{
			format++;
			uint32_t d = va_arg(param, uint32_t);
			if (maxrem < 10)
				return -1;
			tty_puth(d);
			n += 10;
		}
		else
		{
			format = format_begin;
			size_t len = strlen(format);
			if (maxrem < len)
				return -1;
			if (!print(format, len))
				return -1;
			n += len;
			format += len;
		}
	}

	va_end(param);
	return n;
}

int putchar(int ic)
{
	char c = (char)ic;
	tty_putsl(&c, sizeof(c));

	return ic;
}

int puts(const char *s)
{
	return printf("%s\n", s);
}
