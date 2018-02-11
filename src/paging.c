#include "paging.h"

#include "irq.h"
#include "kheap.h"
#include "stdio.h"
#include "string.h"

#define INDEX_FROM_BIT(x) (x/(8*4))
#define OFFSET_FROM_BIT(x) (x%(8*4))

void asm_paging_set_dir(uint32_t*);

extern uint32_t kheap_placement;

mmu_page_dir_t *m_CurrentDir = 0;
mmu_page_dir_t *m_kdir = 0;

uint32_t *m_Frames;
uint32_t mn_Frames;

static void set_frame(uint32_t addr)
{
	uint32_t frame = (addr / MMU_PAGE_SIZE);
	uint32_t i = INDEX_FROM_BIT(frame);
	uint32_t o = OFFSET_FROM_BIT(frame);

	m_Frames[i] |= (MMU_PO_PRESENT << o);
}

static void clear_frame(uint32_t addr)
{
	uint32_t frame = (addr / MMU_PAGE_SIZE);
	uint32_t i = INDEX_FROM_BIT(frame);
	uint32_t o = OFFSET_FROM_BIT(frame);

	m_Frames[i] &= ~(MMU_PO_PRESENT << o);
}

static uint32_t test_frame(uint32_t addr)
{
	uint32_t frame = (addr / MMU_PAGE_SIZE);
	uint32_t i = INDEX_FROM_BIT(frame);
	uint32_t o = OFFSET_FROM_BIT(frame);

	return (m_Frames[i] & (MMU_PO_PRESENT << o));
}

static uint32_t first_frame(void)
{
	for (uint32_t i = 0; i < INDEX_FROM_BIT(mn_Frames); i++)
	{
		if (m_Frames[i] != 0xffffffff)
		{
			for (uint32_t j = 0; j < 32; j++)
			{
				uint32_t t = (MMU_PO_PRESENT << j);
				if (!(m_Frames[i] & t))
					return i*4*8+j;
			}
		}
	}
	return 0;
}

void alloc_frame(mmu_page_t *page, int kernel, int rx)
{
	if (page->frame != 0)
		return;

	uint32_t i = first_frame();

	set_frame(i * MMU_PAGE_SIZE);
	page->present = 1;
	page->rx = (rx) ? 1 : 0;
	page->user = (kernel) ? 0 : 1;
	page->frame = i;
}

void free_frame(mmu_page_t *page)
{
	uint32_t frame;

	if (!(frame = page->frame))
		return;

	clear_frame(frame);
	page->frame = 0x0;
}

void paging_isr(int_reg_t r)
{
	uint32_t at;
	asm volatile("mov %%cr2, %0": "=r"(at));

	int present = (r.err_code & 0x1);
	int rx = (r.err_code & 0x2);
	int user = (r.err_code & 0x4);

	printf("paging_isr: %x e=%s op=%s mode=%s\n",
	at, (present) ? "protected" : "not-present", (rx) ? "read" : "write",
	(user) ? "user" : "kernel");

	while (1) { }
}

void paging_init(void)
{
	uint32_t mem_end = 0x1000000;

	mn_Frames = (mem_end / MMU_PAGE_SIZE);
	m_Frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(mn_Frames));
	memset(m_Frames, 0, INDEX_FROM_BIT(mn_Frames));

	m_kdir = (mmu_page_dir_t*)kmalloc_a(sizeof(mmu_page_dir_t));
	memset(m_kdir, 0, sizeof(mmu_page_dir_t));

	m_CurrentDir = m_kdir;

	uint32_t i = KERNEL_VIRTUAL_BASE;
	while (i < kheap_placement)
	{
		alloc_frame(paging_get_page(i, 1, m_kdir), 0, 0);
		i += MMU_PAGE_SIZE;
	}

	irq_set_handler(14, paging_isr);
	paging_set_dir(m_kdir);
}

void paging_set_dir(mmu_page_dir_t *page_dir)
{
	asm_paging_set_dir((uint32_t*)((uint32_t)&page_dir->table_phys - KERNEL_VIRTUAL_BASE));
}

mmu_page_t *paging_get_page(uint32_t addr, int mk, mmu_page_dir_t *page_dir)
{
	addr /= MMU_PAGE_SIZE;
	uint32_t i = (addr / 1024);

	if (page_dir->table[i])
		return &page_dir->table[i]->page[addr%1024];

	if (mk)
	{
		uint32_t p;
		page_dir->table[i] = (mmu_page_table_t*)kmalloc_ap(sizeof(mmu_page_table_t), &p);
		memset(page_dir->table[i], 0, MMU_PAGE_SIZE);

		page_dir->table_phys[i] = (p | MMU_PO_PRESENT | MMU_PO_WRITE | MMU_PO_USER);

		return &page_dir->table[i]->page[addr%1024];
	}

	return (mmu_page_t*)0;
}
