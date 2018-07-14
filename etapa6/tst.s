.data
.globl  __xXxtempVarxXx123456TEMP0__
.align 4
.type __xXxtempVarxXx123456TEMP0__, @object
.size __xXxtempVarxXx123456TEMP0__, 4
__xXxtempVarxXx123456TEMP0__:
	.long 2

.globl  __xXxtempVarxXx123456TEMP5__
.align 4
.type __xXxtempVarxXx123456TEMP5__, @object
.size __xXxtempVarxXx123456TEMP5__, 4
__xXxtempVarxXx123456TEMP5__:
	.long 25

.globl  b
.align 4
.type b, @object
.size b, 4
b:
	.long 0

.globl  __xXxtempVarxXx123456TEMP3__
.align 4
.type __xXxtempVarxXx123456TEMP3__, @object
.size __xXxtempVarxXx123456TEMP3__, 4
__xXxtempVarxXx123456TEMP3__:
	.long 4

.globl  a
.align 4
.type a, @object
.size a, 4
a:
	.long 0

.globl  __xXxtempVarxXx123456TEMP2__
.align 4
.type __xXxtempVarxXx123456TEMP2__, @object
.size __xXxtempVarxXx123456TEMP2__, 4
__xXxtempVarxXx123456TEMP2__:
	.long 0

.globl  __xXxtempVarxXx123456TEMP1__
.align 4
.type __xXxtempVarxXx123456TEMP1__, @object
.size __xXxtempVarxXx123456TEMP1__, 4
__xXxtempVarxXx123456TEMP1__:
	.long 10

.globl  __xXxtempVarxXx123456TEMP4__
.align 4
.type __xXxtempVarxXx123456TEMP4__, @object
.size __xXxtempVarxXx123456TEMP4__, 4
__xXxtempVarxXx123456TEMP4__:
	.long 0

.text
.globl	main
.type	main, @function
main:
	.cfi_startproc
 	pushq	%rbp
 	movq	%rsp, %rbp
	movl	__xXxtempVarxXx123456TEMP0__(%rip), %eax
 	movl	%eax, a(%rip) 
	movl	__xXxtempVarxXx123456TEMP1__(%rip), %eax
 	movl	%eax, b(%rip) 
	movl	__xXxtempVarxXx123456TEMP2__(%rip), %eax
 	cmpl	__xXxtempVarxXx123456TEMP3__(%rip), %eax 
 	movl	%eax, __xXxtempVarxXx123456TEMP4__(%rip)
	cmpl	$0, __xXxtempVarxXx123456TEMP4__(%rip)
 	je	LABEL__xXxtempVarxXx123456LABEL0__
	movl	__xXxtempVarxXx123456TEMP5__(%rip), %eax
 	movl	%eax, b(%rip) 
LABEL__xXxtempVarxXx123456LABEL0__:
	movl	$0, %eax
 	popq	%rbp
 	ret
 	.cfi_endproc
	.size	main, .-main
