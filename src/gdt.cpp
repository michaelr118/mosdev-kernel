#include "gdt.h"
#include "tss.h"

// _gdt_set() is in boot.s. Much easier to set GDT in assembly
extern "C" { void _gdt_set(); }

// Our actual GDT
struct GDT_ENTRY gdt[4];
// Pointer to GDT used by _gdt_set
struct GDT_PTR _pgdt;

// Task State Segment (TSS), placeholder for now
struct TSS_T tss[1];

/*
 * Insert GDT entry i into the GDT. Each entry must be formatted properly.
 * base - start address of segment
 * limit - end address of segment
 * access - type of segment
*/
static void gdt_entry_set(int i, unsigned long base, unsigned long limit, unsigned char access)
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
 * Initial GDT will be simple with full 4GB untranslated
 * NULL Segment - 0x00, never referenced by the processor
 * Code Segment - 0x08, executable code
 * Data Segment - 0x10, data
 * Task State Segment (TSS) - 0x18, placeholder for now
*/
void gdt_install(void)
{
	// Setting up pointer to gdt. Limit is always sizeof(gdt) - 1
	// base is the starting address of the GDT
	_pgdt.limit = ((sizeof(struct GDT_ENTRY) * 4) - 1);
	_pgdt.base = (unsigned long int)&gdt;

	gdt_entry_set(0, 0, 0, 0); // NULL
	gdt_entry_set(1, 0, 0xffffffff, 0x9a); // Code Segment
	gdt_entry_set(2, 0, 0xffffffff, 0x92); // Data Segment
	gdt_entry_set(3, (unsigned long int)&tss, sizeof(tss), 0x89); // TSS Segment

	// Tells the processor about our new GDT
	_gdt_set();
}
