#ifndef _KERNEL_COMMON_H
#define _KERNEL_COMMON_H

#define CAT_I(a,b) a##b
#define CAT(a,b) CAT_I(a, b)

typedef unsigned int u32int;
typedef int s32int;
typedef unsigned short u16int;
typedef short s16int;
typedef unsigned char u8int;
typedef char s8int;

/*
 * Output byte val to a port on the IO bus
*/
static inline void io_outb(u16int port, u8int val)
{
	asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

/*
 * Read byte from port on the IO bus
*/
static inline u8int io_inb(u16int port)
{
	u8int ret;
	asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
	return ret;
}

/*
 * Wait for an IO operation to complete before proceeding
*/
static inline void io_wait(void)
{
	asm volatile ( "jmp 1f\n\t1:jmp 2f\n\t2:" );
}

#endif
