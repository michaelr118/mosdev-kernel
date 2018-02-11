#ifndef _KERNEL_KHEAP_H
#define _KERNEL_KHEAP_H

#include "common.h"

uint32_t kmalloc(size_t sz);

uint32_t kmalloc_a(size_t sz);
uint32_t kmalloc_p(size_t sz, uint32_t *phys_addr);
uint32_t kmalloc_ap(size_t sz, uint32_t *phys_addr);

#endif
