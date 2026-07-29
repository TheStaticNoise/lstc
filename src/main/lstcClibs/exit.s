.intel_syntax noprefix
.global LSTC_exit
.text

LSTC_exit:
    mov rax, 60
    syscall
