#include "gdt.h"

// _gdt_set() is in boot.s. Much easier to set GDT in assembly
extern "C" { void _gdt_set(u32int); }

// Our actual GDT
gdt_entry_t gdt[5];
// Pointer to GDT used by _gdt_set
gdt_ptr_t gdt_ptr;

/*
 * Insert GDT entry i into the GDT. Each entry must be formatted properly.
 * base - start address of segment
 * limit - end address of segment
 * access - type of segment
*/
static void gdt_entry_set(s32int i, u32int base, u32int limit, u8int access)
{
	// GDT expects base address to be split into low, middle, and high
	gdt[i].base_l = (base & 0xffff);
	gdt[i].base_middle = ((base >> 16) & 0xff);
	gdt[i].base_high = ((base >> 24) & 0xff);

	// We only need the lower half of the actual limit
	gdt[i].limit_l = (limit & 0xffff);

	// Granularity 0xcf could be changed here, but we'll leave it all default
	gdt[i].gran = ((limit >> 16) & 0x0f);
	gdt[i].gran |= (0xcf & 0xf0);

	// Finally, set the type of the segment
	gdt[i].access = access;
}

/*
 * Installs our GDT (Global Descriptor Table).
 * Initial GDT will be simple and use full 4GB untranslated
*/
void gdt_install(void)
{
	// Setting up pointer to gdt. Limit is always sizeof(gdt) - 1
	// base is the starting address of the GDT
	gdt_ptr.limit = ((sizeof(gdt_entry_t) * 5) - 1);
	gdt_ptr.base = (addr)&gdt;

	gdt_entry_set(0, 0, 0, 0); // NULL

	gdt_entry_set(1, 0, 0xffffffff, 0x9a); // Kernel Code Segment
	gdt_entry_set(2, 0, 0xffffffff, 0x92); // Kernel Data Segment

	gdt_entry_set(3, 0, 0xffffffff, 0xfa); // User Code Segment
	gdt_entry_set(4, 0, 0xffffffff, 0xf2); // User Data Segment

	// Tells the processor about our new GDT
	_gdt_set((u32int)&gdt_ptr);
}
