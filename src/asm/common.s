; common.s

section .text

; asm_gdt_set(uint32_t gdt_ptr)
; Sets the Global Descriptor Table (GDT)

global asm_gdt_set
asm_gdt_set:
	mov eax, [esp+4]
	lgdt [eax]

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	jmp 0x08:_gdt_set_ret

_gdt_set_ret:
	ret

; asm_idt_set(uint32_t idt_ptr)
; Sets the Interrupt Descriptor Table (IDT)

global asm_idt_set
asm_idt_set:
	mov eax, [esp+4]
	lidt [eax]
	ret

; asm_paging_set_dir(uint32_t*)
; Sets the page directory

global asm_paging_set_dir
asm_paging_set_dir:
	push ebp
	mov ebp, esp
	mov eax, [esp+8]
	mov cr3, eax
	mov esp, ebp
	pop ebp
	ret
