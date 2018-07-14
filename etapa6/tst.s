	.file	"tst.c"
	.globl	a
	.data
	.align 4
	.type	a, @object
	.size	a, 4
a:
	.long	1
	.globl	b
	.align 4
	.type	b, @object
	.size	b, 4
b:
	.long	1
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	a(%rip), %edx
	movl	b(%rip), %eax
	cmpl	%eax, %edx
	jne	.L2
	movl	$24, a(%rip)
.L2:
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits

\tmovl\t%s(%%rip), %%edx\n\tmovl\t%s(%%rip), %%eax\n\tsubl\t%%eax, %%edx\n\torl\t$0, %%edx\n