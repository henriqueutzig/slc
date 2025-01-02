	.file	"test_base.c"
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$2, -8(%rbp)
	movl	$20, -4(%rbp)
	movl	-8(%rbp), %eax
	addl	%eax, -4(%rbp)
	movl	-8(%rbp), %eax
	popq	%rbp
	ret
