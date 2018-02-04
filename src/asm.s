/*
asm.s
Contains assembly language for the kernel
*/

.section .text

/*
asm_gdt_set(u32int gdt_ptr)
Sets the Global Descriptor Table (GDT)
*/
.global _gdt_set
.type _gdt_set, @function

_gdt_set:
	mov 4(%esp), %eax
	lgdt (%eax)
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	jmp $0x08, $_gdt_set_ret

_gdt_set_ret:
	ret

/*
asm_idt_set(u32int idt_ptr)
Sets the Interrupt Descriptor Table (IDT)
*/
.global _idt_set
.type _idt_set, @function

_idt_set:
	mov 4(%esp), %eax
	lidt (%eax)
	ret

/*
Interrupt Service Routine (IRS) macros.
The first 32 are absolutely required and should not be null.
0...7 and 15...31 Give no error codes, push 0 to keep the stack consistent
8 and 10...14 provide an error code, push that onto the stack instead.
*/
.macro ISR_NOERR val
.global isr\val
isr\val:
	cli
	push $0
	push $\val
	jmp isr_common
.endm

.macro ISR_ERR val
.global isr\val
isr\val:
	cli
	push $\val
	jmp isr_common
.endm

// Division by zero
ISR_NOERR 0

// Single-step interrupt
ISR_NOERR 1

// NMI
ISR_NOERR 2

// Breakpoint (debuggers)
ISR_NOERR 3

// Overflow
ISR_NOERR 4

// Bounds error
ISR_NOERR 5

// Invalid opcode
ISR_NOERR 6

// Coprocessor not available
ISR_NOERR 7

// Double fault
ISR_ERR 8

// Coprocessor segment overrun
ISR_NOERR 9

// Invaid TSS
ISR_ERR 10

// Segment not present
ISR_ERR 11

// Stack fault
ISR_ERR 12

// General protection fault
ISR_ERR 13

// Page fault
ISR_ERR 14

// Rsvd
ISR_NOERR 15

// Math fault
ISR_NOERR 16

// Alignment check
ISR_NOERR 17

// Machine check
ISR_NOERR 18

// Floating-point exception
ISR_NOERR 19

// Virtualization exception
ISR_NOERR 20

// Control protection exception
ISR_NOERR 21

// Rsvd
ISR_NOERR 22
// Rsvd
ISR_NOERR 23
// Rsvd
ISR_NOERR 24
// Rsvd
ISR_NOERR 25

// Rsvd
ISR_NOERR 26
// Rsvd
ISR_NOERR 27
// Rsvd
ISR_NOERR 28
// Rsvd
ISR_NOERR 29

// Rsvd
ISR_NOERR 30
// Rsvd
ISR_NOERR 31

// isr_handler found in isr.cpp
.extern isr_handler

/*
Calls isr_handler with the current state of the registers, the interrupt number,
and the error code (if any).
*/
isr_common:
	pusha
	mov %ds, %ax
	push %eax
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	call isr_handler
	pop %eax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	popa
	add $8, %esp
	sti
	iret
