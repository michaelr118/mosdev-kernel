/*
idt.s
Contains assembly language for the IDT
*/

.section .text

/*
_idt_set(u32int idt_ptr)
Sets the Interrupt Descriptor Table (IDT)
*/
.global _idt_set
.type _idt_set, @function

_idt_set:
	mov 4(%esp), %eax
	lidt (%eax)
	ret
