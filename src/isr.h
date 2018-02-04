#ifndef _KERNEL_ISR_H
#define _KERNEL_ISR_H

#include "common.h"

typedef struct ISR_REG {
	u32int ds;
	u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u32int int_no, err_code;
	u32int eip, cs, opt, uesp, ss;
} isr_reg_t;

#endif
