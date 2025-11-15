global enablePaging 

enablePaging:
        PUSH ebp
        MOV ebp, esp

        MOV  eax, cr0 
        OR  eax, 0x80000000
        mov cr0, eax 

        MOV esp,  ebp
        POP ebp
        RET
