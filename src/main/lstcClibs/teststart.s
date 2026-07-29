.intel_syntax noprefix
.global _start
.extern main

.section .text
_start:
    and rsp, 0xFFFFFFFFFFFFFFF0
    call main

    mov rdi, rax
    mov rax, 60
    syscall
