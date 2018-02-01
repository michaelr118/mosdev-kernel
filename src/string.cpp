#include "string.h"

int memcmp(const void *aptr, const void *bptr, size_t size)
{
	const unsigned char *a = (const unsigned char*)aptr;
	const unsigned char *b = (const unsigned char*)bptr;

	for (size_t i = 0; i < size; i++)
	{
		if (a[i] < b[i])
			return 1;
		else if (b[i] < a[i])
			return 1;
	}

	return 0;
}

void *memcpy(void* __restrict pdst, const void* psrc, size_t size)
{
	unsigned char *dst = (unsigned char*)pdst;
	const unsigned char *src = (const unsigned char*)psrc;

	for (size_t i = 0; i < size; i++)
		dst[i] = src[i];

	return pdst;
}

void *memmove(void *pdst, const void *psrc, size_t size)
{
	unsigned char *dst = (unsigned char*)pdst;
	const unsigned char *src = (const unsigned char*)psrc;

	if (dst < src)
	{
		for (size_t i = 0; i < size; i++)
			dst[i] = src[i];
	}
	else
	{
		for (size_t i = size; i != 0; i--)
			dst[i-1] = src[i-1];
	}

	return pdst;
}

void *memset(void *ptr, int value, size_t size)
{
	unsigned char *dst = (unsigned char*)ptr;
	for (size_t i = 0; i < size; i++)
		dst[i] = (unsigned char)value;

	return ptr;
}

size_t strlen(const char *str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
