#ifndef _KERNEL_COMMON_H
#define _KERNEL_COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define KERNEL_VIRTUAL_BASE 0xc0000000

/*
 * Output byte val to a port on the IO bus
*/
static inline void io_outb(uint16_t port, uint8_t val)
{
	asm volatile("outb %0, %1":: "a"(val), "Nd"(port));
}

/*
 * Read byte from port on the IO bus
*/
static inline uint8_t io_inb(uint16_t port)
{
	uint8_t ret;
	asm volatile("inb %1, %0": "=a"(ret): "Nd"(port));
	return ret;
}

/*
 * Wait for an IO operation to complete before proceeding
*/
static inline void io_wait(void)
{
	asm volatile("jmp 1f\n\t1:jmp 2f\n\t2:");
}

#endif
