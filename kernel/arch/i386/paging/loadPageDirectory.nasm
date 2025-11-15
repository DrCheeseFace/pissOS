global loadPageDirectory 

loadPageDirectory:
        PUSH ebp
        MOV ebp, esp

        MOV  eax, [esp + 8]
        MOV cr3, eax

        MOV esp,  ebp
        POP ebp
        RET
