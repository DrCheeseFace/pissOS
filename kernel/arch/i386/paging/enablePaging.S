.global _enablePaging

_enablePaging:
        pushl %ebp
        movl %esp, %ebp

        movl %cr0, %eax
        orl $0x80000000, %eax
        movl %eax, %cr0

        movl %ebp, %esp
        popl %ebp
        ret
