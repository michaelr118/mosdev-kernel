/*
crtn.s
No code here. Only used to link the C-runtime to our kernel.
*/

.section .init
	popl %ebp
	ret

.section .fini
	popl %ebp
	ret
