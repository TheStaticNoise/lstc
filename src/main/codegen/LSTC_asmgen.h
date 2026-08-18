#ifndef LSTC_ASMGEN
#define LSTC_ASMGEN

// NASM syntax needed

/* (this also works as a fucking nasm guide)
 * at begining use ```format ELF64```
 sections:
  syntax: SECTION [NAME]
  : .data
  : .text
  : .rodata
  : .bss
  rodata: read only
  data: just data
  bss: reserved memory (zeroed out)
 includes:
  %include        '[name].asm'
  includes every symbol from [name].asm and functions, better than ld (XD)
 labels:
  global [label] - set label to be visible to everyone
  extern [label] - we don't have it right now, but we'll get it during linking
  syntax:
  [LABEL NAME]:
    {code}
  behavior: instructions are linear, it can easily fall thru to next label (this is why sys_exit exists)
 instructions:
    stack related:
        push [reg/memory] - push onto stack
        pop [reg/memory] - retrieve last stack element
        call [label] - pushes return address onto stack and jumps
        ret - returns to caller (never use on entry point function, by std its called _start, returning from it returns to garbage, use SYS_EXIT)
    general:
        e - register or memory
        em - register, memory, or value
        mov [e], [em]    : moves to register/memory, (e = em)
        add [e], [em]
        sub [e], [em]    : [e] - [em]
        inc [e]          : increase (++)
        dec [e]          : decrease (--)
        cmp [em], [em]   : compare
        int [em]         : interrupt [em]
        mul  [em]   : rax = rax * [em] (unsigned), result in rdx:rax (high:low)
        imul [em]   : rax = rax * [em] (signed),   result in rdx:rax (high:low)
        div  [em]   : rdx:rax / [em] (unsigned), quotient -> rax, remainder -> rdx
                      MUST zero rdx first (xor rdx, rdx) or garbage/#DE
        idiv [em]   : rdx:rax / [em] (signed),   quotient -> rax, remainder -> rdx
                      MUST sign-extend rdx first (cqo) or garbage/#DE
        jmp              : unconditional jump, no stack
        j[suffix]:
            jump if - used after cmp
            suffixes:
                signed:
                    z  : equals 0
                    l  : lower than second param
                    g  : greater than second param
                    ge : greater or equals to second param
                    le : lower or equals to second param
                    ne : not equals to second param
                unsigned:
                    a  : jump if > cond2
                    b  : inverse of ja
                    e  : == cond2
                    ae : inherit a || inherit e
                    be : inherit b || inherit e
    data:
        .data:
            string:
                name db 'STRING'(, num or another string)
                example: msg db ''
 */



#endif
