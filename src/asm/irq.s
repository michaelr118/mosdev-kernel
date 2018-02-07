.section .text

/*
Interrupt Service Request (IRQ) macros.
An IRQ is generated when something needs the kernel's attention (such
as the keyboard, mouse, etc).
The first 15 of these are BIOS interrupts re-mapped to 32+
*/
.macro IRQ i, j
.global irq\i
irq\i:
	cli
	push $0
	push $\j
	jmp irq_common
.endm

// Programmable Interrupt Timer
IRQ 0,32
// Keyboard Input
IRQ 1,33
// Cascade PIC
IRQ 2,34
// COM2
IRQ 3,35
// COM1
IRQ 4,36
// LPT2
IRQ 5,37
// Floppy Disk
IRQ 6,38
// LPT1
IRQ 7,39
// CMOS Clock
IRQ 8,40
// Peripheral
IRQ 9,41
// Peripheral
IRQ 10,42
// Peripheral
IRQ 11,43
// PS2 Mouse
IRQ 12,44
// Coprocessor
IRQ 13,45
// Primary ATA HDD
IRQ 14,46
// Secondary ATA HDD
IRQ 15,47

.extern irq_handler

/*
Calls irq_handler with the current state of the registers and the interrupt number.
irq_handler will be responsible for dispatching messages to the registered handler.
If we don't have a registered handler, the IRQ is simply ignored.
*/
irq_common:
	pusha
	push %ds
	push %es
	push %fs
	push %gs

	mov $0x10, %ax

	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	mov %esp, %eax
	push %eax

	mov $irq_handler, %eax
	call %eax

	pop %eax
	pop %gs
	pop %fs
	pop %es
	pop %ds
	popa
	add $8, %esp
	sti
	iret
