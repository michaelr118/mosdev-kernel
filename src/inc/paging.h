#ifndef _KERNEL_PAGING_H
#define _KERNEL_PAGING_H

#include "common.h"
#define MMU_PAGE_SIZE 0x1000

typedef struct MMU_PAGE {
	uint32_t present : 1;
	uint32_t rx : 1;
	uint32_t user : 1;
	uint32_t accessed : 1;
	uint32_t dirty : 1;
	uint32_t rsvd : 7;
	uint32_t frame : 20;
} mmu_page_t;

typedef struct MMU_PAGE_TABLE {
	mmu_page_t page[1024];
} mmu_page_table_t;

typedef struct MMU_PAGE_DIR {
	mmu_page_table_t *table[1024];
	uint32_t table_phys[1024];
} mmu_page_dir_t;

typedef enum MMU_PAGE_OPT {
	MMU_PO_PRESENT = 0x1,
	MMU_PO_WRITE = 0x2,
	MMU_PO_USER = 0x4,
	MMU_PO_ACCESSED = 0x32,
	MMU_PO_DIRTY = 0x64
} mmu_page_opt_t;

void paging_init(void);

void paging_set_dir(mmu_page_dir_t *page_dir);
mmu_page_t *paging_get_page(uint32_t addr, int mk, mmu_page_dir_t *page_dir);

#endif
