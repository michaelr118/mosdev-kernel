#ifndef _KERNEL_IRQ_H
#define _KERNEL_IRQ_H

#include "common.h"
#include "intreg.h"

typedef void (*irq_handler_t)(int_reg_t);

void irq_init(void);
void irq_set_handler(u8int irq_number, irq_handler_t handler);

#endif
