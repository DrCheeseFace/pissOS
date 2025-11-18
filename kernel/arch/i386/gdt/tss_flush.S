.global tss_flush
.type tss_flush, @function

tss_flush:
    movl $0x2B, %eax
    LTR %ax
    RET
