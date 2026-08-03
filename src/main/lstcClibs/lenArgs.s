.text
.globl LSTC_getVariadicPTR

LSTC_getVariadicPTR:
    mov %rsp, %rax
    add $48, %rax
    ret
