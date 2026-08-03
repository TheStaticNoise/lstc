.intel_syntax noprefix
.global _start
.extern LSTC_main

.section .text
_start:
    mov rdi, rsp
    and rsp, -16
    call LSTC_main

    mov rdi, rax
    mov rax, 60
    syscall
