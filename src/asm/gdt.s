/*
gdt.s
Contains assembly language for the GDT
*/

.section .text

/*
_gdt_set(u32int gdt_ptr)
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
