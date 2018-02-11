global _boot_main
extern kernel_main

MBOOT_PAGE_ALIGN equ (1 << 0)
MBOOT_MEM_INFO equ (1 << 1)
MBOOT_HEADER_OPT equ (MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO)
MBOOT_HEADER_MAGIC equ 0x1badb002
MBOOT_CHECKSUM equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_OPT)

KERNEL_VIRTUAL_BASE equ 0xc0000000
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22)

section .data
align 0x1000

boot_page_dir:
	dd 0x00000083
	times (KERNEL_PAGE_NUMBER - 1) dd 0
	dd 0x00000083
	times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0

section .text
align 4

mb_header:
	dd MBOOT_HEADER_MAGIC
	dd MBOOT_HEADER_OPT
	dd MBOOT_CHECKSUM

_boot_main:
	mov ecx, (boot_page_dir - KERNEL_VIRTUAL_BASE)
	mov cr3, ecx

	mov ecx, cr4
	or ecx, 0x00000010
	mov cr4, ecx

	mov ecx, cr0
	or ecx, 0x80000000
	mov cr0, ecx

	lea ecx, [_high_main]
	jmp ecx

_high_main:
	mov dword [boot_page_dir], 0
	invlpg [0]

	mov esp, stack_top
	push eax
	push ebx

	call kernel_main
	hlt

section .bss
align 32

stack_bottom:
resb 16384
stack_top:
