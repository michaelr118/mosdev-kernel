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

.section .bss

.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
	
.global _boot_main
.type _boot_main, @function

_boot_main:
	mov $stack_top, %esp

	call _init
	call kernel_main

	cli
1:	hlt
	jmp 1b

// Useful for debugging and call tracing
.size _boot_main, (. - _boot_main)
