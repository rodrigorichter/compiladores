.text
.globl	main
.type	main, @function
main:
.cfi_startproc
pushq	%rbp
movq	%rsp, %rbp
movl	10, a(%rbp) 
movl	10, b(%rbp) 
movl	b(%rbp), %eax 
cltd
idivl	a(%rbp) 
movl	%eax, __xXxtempVarxXx123456TEMP(0)__(%rbp) 
movl	$0, %eax
popq	%rbp
ret
.cfi_endproc
