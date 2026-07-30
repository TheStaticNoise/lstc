#on an incredible journey to shave off stdlib bloat
.intel_syntax noprefix
.global LSTC_write, LSTC_read, LSTC_open, LSTC_close, LSTC_lseek
## Quick guide, system-V abi does this:
# arg1 = rdi
# arg2 = rsi
# arg3 = rdx
# arg4 = rcx // overwriten by fucking syscall, never leave your arg here when calling syscall, the cpu is evil, and linux uses r10 for 4th syscall arg
# arg5 = r8
# arg6 = r9
# and so on, r12-r15 is reserved by caller, but, you can use them if you restore them (this is called spilling, saving
# registers to memory, and then restoring them)

# int fd, char* buf, size_t ctn -> return size_t written (rax)
LSTC_write:
    mov eax, 1
    syscall
    ret
# int fd, char* buf, size_t ctn -> return size_t read (rax)
LSTC_read:
    xor eax, eax # optimization
    syscall
    ret
# imut char* name, flags, mode -> return int fd (rax) || errno (negative val)
LSTC_open:
    mov eax, 2
    syscall
    ret
#int fd -> int 0 success || errno (negative val)
LSTC_close:
    mov eax, 3
    syscall
    ret
#int fd, size_t offset, uint whence -> size_t
LSTC_lseek:
    mov eax, 8
    syscall
    ret
