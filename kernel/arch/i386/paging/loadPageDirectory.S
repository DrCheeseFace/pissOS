.global _loadPageDirectory

_loadPageDirectory:
	pushl	%ebp
	movl	%esp, %ebp

	movl	8(%ebp), %eax
	movl	%eax, %cr3

	movl	%ebp, %esp
	popl	%ebp
	ret
