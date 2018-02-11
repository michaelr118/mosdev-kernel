section .text

; Interrupt Service Routine (IRS) macros.
; The first 32 are absolutely required and should not be null.
; 0...7 and 15...31 Give no error codes, push 0 to keep the stack consistent
; 8 and 10...14 provide an error code, push that onto the stack instead.
%macro ISR_NOERR 1
global isr%1
isr%1:
	push byte 0
	push %1
	jmp isr_common
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
	push byte %1
	jmp isr_common
%endmacro

; Division by zero
ISR_NOERR 0

; Single-step interrupt
ISR_NOERR 1

; NMI
ISR_NOERR 2

; Breakpoint (debuggers)
ISR_NOERR 3

; Overflow
ISR_NOERR 4

; Bounds error
ISR_NOERR 5

; Invalid opcode
ISR_NOERR 6

; Coprocessor not available
ISR_NOERR 7

; Double fault
ISR_ERR 8

; Coprocessor segment overrun
ISR_NOERR 9

; Invaid TSS
ISR_ERR 10

; Segment not present
ISR_ERR 11

; Stack fault
ISR_ERR 12

; General protection fault
ISR_ERR 13

; Page fault
ISR_ERR 14

; Rsvd
ISR_NOERR 15

; Math fault
ISR_NOERR 16

; Alignment check
ISR_ERR 17

; Machine check
ISR_NOERR 18

; Floating-point exception
ISR_NOERR 19

; Virtualization exception
ISR_NOERR 20

; Control protection exception
ISR_NOERR 21

; Rsvd
ISR_NOERR 22
; Rsvd
ISR_NOERR 23
; Rsvd
ISR_NOERR 24
; Rsvd
ISR_NOERR 25

; Rsvd
ISR_NOERR 26
; Rsvd
ISR_NOERR 27
; Rsvd
ISR_NOERR 28
; Rsvd
ISR_NOERR 29

; Rsvd
ISR_ERR 30
; Rsvd
ISR_NOERR 31

; isr_handler found in isr.cpp
extern isr_handler

; Calls isr_handler with the current state of the registers, the interrupt number,
; and the error code (if any).
isr_common:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, isr_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret

; Interrupt Service Request (IRQ) macros.
; An IRQ is generated when something needs the kernel's attention (such
; as the keyboard, mouse, etc).
; The first 15 of these are BIOS interrupts re-mapped to 32+
%macro IRQ 2
global irq%1
irq%1:
	push byte 0
	push byte %2
	jmp irq_common
%endmacro

; Programmable Interrupt Timer
IRQ 0,32
; Keyboard Input
IRQ 1,33
; Cascade PIC
IRQ 2,34
; COM2
IRQ 3,35
; COM1
IRQ 4,36
; LPT2
IRQ 5,37
; Floppy Disk
IRQ 6,38
; LPT1
IRQ 7,39
; CMOS Clock
IRQ 8,40
; Peripheral
IRQ 9,41
; Peripheral
IRQ 10,42
; Peripheral
IRQ 11,43
; PS2 Mouse
IRQ 12,44
; Coprocessor
IRQ 13,45
; Primary ATA HDD
IRQ 14,46
; Secondary ATA HDD
IRQ 15,47

extern irq_handler

; Calls irq_handler with the current state of the registers and the interrupt number.
; irq_handler will be responsible for dispatching messages to the registered handler.
; If we don't have a registered handler, the IRQ is simply ignored.
irq_common:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, irq_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret
