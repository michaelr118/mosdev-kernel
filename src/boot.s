/* MULTIBOOT HEADER */
.set ALIGN, 1 << 0
.set MEMINFO, 1 << 1
.set OPT, (ALIGN | MEMINFO)
.set MAGIC, 0x1badb002
.set CHECKSUM, -(MAGIC + OPT)

.section .multiboot

.align 4
.long MAGIC
.long OPT
.long CHECKSUM

/* END MULTIBOOT HEADER */

.section .bss

.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text

.extern _pgdt

.global _gdt_set
.type _gdt_set, @function

_gdt_set:
	lgdt (_pgdt)
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	jmp $0x08, $_gdt_set_ret

_gdt_set_ret:
	ret

.global _tss_flush
.type _tss_flush, @function

_tss_flush:
	mov $0x2b, %ax
	ltr %ax
	ret

.global _boot_main
.type _boot_main, @function

_boot_main:
	mov $stack_top, %esp

	// TODO: Setup GDT/paging

	call _init
	call kernel_main

	cli
1:	hlt
	jmp 1b

// Useful for debugging and call tracing
.size _boot_main, (. - _boot_main)
