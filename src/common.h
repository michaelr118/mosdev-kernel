#ifndef _KERNEL_COMMON_H
#define _KERNEL_COMMON_H

typedef unsigned int u32int;
typedef int s32int;
typedef unsigned short u16int;
typedef short s16int;
typedef unsigned char u8int;
typedef char s8int;

static inline void io_outb(u16int port, u8int val)
{
	asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline u8int io_inb(u16int port)
{
	u8int ret;
	asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
	return ret;
}

#endif
