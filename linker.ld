/* Entry-point for the bootloader */
ENTRY(_boot_main)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
	/* Kernel will be loaded at 3GB + 1MB by the bootloader */
	. = 0xc0100000;

	/* Multiboot header and kernel code */
	.text : AT(ADDR(.text) - 0xc0000000)
	{
		_text = .;
		*(.text)
		*(.rodata*)
	}

	/* Read/Write (initialized) */
	.data ALIGN(0x1000) : AT(ADDR(.data) - 0xc0000000)
	{
		_data = .;
		*(.data)
	}

	/* Read/Write (uninitialized) */
	.bss : AT(ADDR(.bss) - 0xc0000000)
	{
		_bss = .;
		*(COMMON)
		*(.bss)
	}
	_end = .;
}
