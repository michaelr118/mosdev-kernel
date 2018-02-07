#ifndef _KERNEL_INT_REG_H
#define _KERNEL_INT_REG_H

typedef struct INT_REG {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, opt, uesp, ss;
} int_reg_t;

#endif
