#include "kheap.h"

extern uint32_t _end;
uint32_t kheap_placement = (uint32_t)&_end;

static uint32_t kmalloc_int(size_t sz, int align, uint32_t *phys_addr)
{
	if (align && (kheap_placement & 0xfffff000))
	{
		kheap_placement &= 0xfffff000;
		kheap_placement += 0x1000;
	}

	if (phys_addr)
		*phys_addr = (kheap_placement - KERNEL_VIRTUAL_BASE);

	uint32_t p = kheap_placement;
	kheap_placement += sz;

	return p;
}

uint32_t kmalloc(size_t sz)
{
	return kmalloc_int(sz, 0, (uint32_t*)0);
}

uint32_t kmalloc_a(size_t sz)
{
	return kmalloc_int(sz, 1, (uint32_t*)0);
}

uint32_t kmalloc_p(size_t sz, uint32_t *phys_addr)
{
	return kmalloc_int(sz, 0, phys_addr);
}

uint32_t kmalloc_ap(size_t sz, uint32_t *phys_addr)
{
	return kmalloc_int(sz, 1, phys_addr);
}
