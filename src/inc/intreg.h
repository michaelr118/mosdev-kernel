#ifndef _KERNEL_INT_REG_H
#define _KERNEL_INT_REG_H

typedef struct INT_REG {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, rsvd, ebx, edx, ecx, eax;
	unsigned int err_code, int_no;
	unsigned int eip, cs, opt, esp, ss;
} int_reg_t;

#endif
