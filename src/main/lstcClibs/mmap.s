.text
.globl LSTC_mmap, LSTC_munmap
# x86_64 linux MMAP asm code, AT&T syntax
LSTC_mmap:
mov %rcx, %r10
mov $9, %eax
syscall
ret
LSTC_munmap:
mov $11, %eax
syscall
ret
#all that cuz C asm sucks, what you mean I can't have almost all the registers???
