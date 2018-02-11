#include "gdt.h"

gdt_entry_t m_GDT[5];
gdt_ptr_t mp_GDT;

void asm_gdt_set(uint32_t);

/*
 * Insert GDT entry i into the GDT. Each entry must be formatted properly.
 * base - start address of segment
 * limit - end address of segment
 * access - type of segment
*/
static void gdt_entry_set(int32_t i, uint32_t base, uint32_t limit, uint8_t access)
{
	// GDT expects base address to be split into low, middle, and high
	m_GDT[i].base_l = (base & 0xffff);
	m_GDT[i].base_middle = ((base >> 16) & 0xff);
	m_GDT[i].base_high = ((base >> 24) & 0xff);

	// We only need the lower half of the actual limit
	m_GDT[i].limit_l = (limit & 0xffff);

	// Granularity 0xcf could be changed here, but we'll leave it all default
	m_GDT[i].gran = ((limit >> 16) & 0x0f);
	m_GDT[i].gran |= (0xcf & 0xf0);

	// Finally, set the type of the segment
	m_GDT[i].access = access;
}

/*
 * Installs our GDT (Global Descriptor Table).
 * Initial GDT will be simple and use full 4GB untranslated
*/
void gdt_init(void)
{
	// Setting up pointer to gdt. Limit is always sizeof(gdt) - 1
	// base is the starting address of the GDT
	mp_GDT.limit = ((sizeof(gdt_entry_t) * 5) - 1);
	mp_GDT.base = (uint32_t)(&m_GDT - KERNEL_VIRTUAL_BASE);

	gdt_entry_set(0, 0, 0, 0); // NULL

	gdt_entry_set(1, 0, 0xffffffff, 0x9a); // Kernel Code Segment
	gdt_entry_set(2, 0, 0xffffffff, 0x92); // Kernel Data Segment

	gdt_entry_set(3, 0, 0xffffffff, 0xfa); // User Code Segment
	gdt_entry_set(4, 0, 0xffffffff, 0xf2); // User Data Segment

	// Tells the processor about our new GDT
	asm_gdt_set((uint32_t)&mp_GDT);
}
