#ifndef LSTC_ASMGEN
#define LSTC_ASMGEN
#include "../lstcClibs/LSTC_primitives.h"
#include "../lstcClibs/LSTC_pager.h"

// simple fasm generator + IR

// FASM syntax needed

/* (this also works as a fucking fasm guide) (rebranded)
 * at begining use ```format ELF64 3 executable```
 sections:
  syntax: SECTION [identity]
  : data readable writeable
  : readable executable
  : readable
  rodata: read only
  data: just data
  bss: reserved memory (zeroed out)
 includes:
  include        '[name].asm'
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
                    nz : != 0
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

// btw btw, the asmgen doesn't care about your name mangling, it just stores tokens and then spits out fasm code





typedef enum {
    // registers (format - register and then variant)
    // variants
    // 0 - 64 bits
    // 1 - 32 bits
    // 2 - 16 bits
    // 3 - 8/16 high
    // 4 - 8 bits
    GEN_REG, // signature token + regNum + variant (ref: registers)
    // variants
    // 0 : 512 (32 total registers, 0 - 31)
    // 1 : 256 (16 total, 0-15)
    // 2 : 128 (same)
    SIMD_REG, // signature token + regNum + variant
    // data
    MOVE,
    SUB,
    DEC,
    INC,
    ADD,
    MUL,
    IMUL,
    DIV,
    IDIV,
    // stack
    CALL,
    RET,
    PUSH,
    POP,
    // types
    DEF, // preceeded by a class type (Struct/function) and a literal number after
    STRUCT,
    FUNCTION,
    // logic
    XOR,
    AND,
    NOT,
    OR,
    COMP,
    // JUMP
    JUMP, // Jump + jump type
    // JUMP CONDS
    LOWER,
    GREATER,
    ZERO,
    NOT_ZERO,
    GREATER_EQ,
    LOWER_EQ,
    NOT_EQUAL,
    EQUAL,
    ABOVE,
    BELOW,
    ABOVE_EQ,
    BELOW_EQ,
    LEA,
    // virtual
    UNWIND, // signature token + id to declare, token + jump cond + id to jump back to UNWIND ID
    // system
    SYSCALL
} LSTC_ASMGEN_TOKENS;

typedef enum
{ MEM, LIT, REF, REG }
LSTC_asm_datatype;

typedef enum
{ X86_64 } // yes, only 1
LSTC_asm_arch;


typedef struct {
    LSTC_asm_datatype type;
    LSTC_asm_datatype base_type;
    LSTC_asm_datatype index_type;
    LSTC_asm_datatype scale_type;
    LSTC_asm_datatype disp_type;
    L__64 value; // for memory it is base
    // mem segment
    L__64 index;
    int   scale;
    L__64 disp;
} LSTC_asm_operand;

typedef struct {
    void* token_page;
    void** operands_pages;
    char* name;
    L__64 name_sz;
    L__64 op_pagec;
    L__64 instruction_count;
} LSTC_asm_function;

typedef struct {
    enum {DB, DW, DD, DQ, DT} element_size;
    enum {STR, HEX, PU64, P_HEX} lit_type;
    void* literal;
    L__64 size;
} LSTC_asm_rodata;

typedef struct {
    LSTC_asm_operand** operands_pages;
    L__64 operands_page_count; // how many pages
    L__64 operands_page_size; // how big
    L__64 operands_count; // how much actually used
    LSTC_asm_function** functions;
    L__64 functions_page_count; // how many pages
    L__64 functions_page_size; // how big
    L__64 functions_count; // how much actually used
    LSTC_asm_function* entry;
    void* app_name; // = 1 if error (XD, anything just not to add another field)
} LSTC_asm_container;

[[nodiscard]] // love GNU extensions
static LSTC_asm_container LSTC_asm_container_create(L__64 op_count, L__64 op_pages, L__64 fn_count, L__64 fn_pages) {
    LSTC_asm_container e = {
        .operands_pages = 0,
        .operands_page_count = op_pages,
        .operands_page_size = op_count,
        .operands_count = 0,
        .functions_count = 0,
        .functions = 0,
        .functions_page_count = fn_pages,
        .functions_page_size = fn_count,
        .entry = 0,
        .app_name = 0,
    };
    e.functions = LSTC_simplify_mmap_private(fn_pages * 8);
    if (e.functions == 0) {
        e.app_name = ptrCast 1;
        return e;
    }
    for (L__64 i = 0; i < fn_pages; i++) {
        e.functions[i] = LSTC_simplify_mmap_private(fn_count * sizeof(LSTC_asm_function));
        if (e.functions[i] == 0) {
            e.app_name = ptrCast 1;
            return e;
        }
    }

    e.operands_pages = LSTC_simplify_mmap_private(op_pages * 8);
    if (e.operands_pages == 0) {
        e.app_name = ptrCast 1;
        return e;
    }
    for (L__64 i = 0; i < op_pages; i++) {
        e.operands_pages[i] = LSTC_simplify_mmap_private(op_count * sizeof(LSTC_asm_operand));
        if (e.operands_pages[i] == 0) {
            e.app_name = ptrCast 1;
            return e;
        }
    }
    return e;
}

static void LSTC_asm_container_delete(LSTC_asm_container* ctx) {
    if (!ctx) return;
    ctx->app_name = 0;
    ctx->entry = 0;
    for (int i = 0; i < ctx->functions_page_count; i++) {
        if (ctx->functions[i]) {
            LSTC_munmap(ctx->functions[i], ctx->functions_page_size);
        }
    }
    if (ctx->functions) {
        LSTC_munmap(ctx->functions, ctx->functions_page_count * sizeof(void*));
    }
    for (int i = 0; i < ctx->operands_page_count; i++) {
        if (ctx->operands_pages[i]) {
            LSTC_munmap(ctx->operands_pages[i], ctx->operands_page_size);
        }
    }
    if (ctx->operands_pages) {
        LSTC_munmap(ctx->operands_pages, ctx->operands_page_count * sizeof(void*));
    }
    ctx->functions_page_count = 0;
    ctx->functions_page_size = 0;
    ctx->functions_count = 0;
    ctx->operands_count = 0;
    ctx->operands_page_count = 0;
    ctx->operands_pages = 0;
    ctx->functions = 0;
}

typedef struct {
    L__64 PAGE;
    L__64 INDX;
}
LSTC_asmgen_indx;

static inline LSTC_asmgen_indx LSTC_ASMGEN_helperIndx_fn(LSTC_asm_container *ctx, L__64 e)
{
    LSTC_asmgen_indx r;

    r.PAGE = e / ctx->functions_page_size;
    r.INDX = e % ctx->functions_page_size;

    return r;
}
[[nodiscard]]
static void* LSTC_asm_create_function(LSTC_asm_container* ctx, char* name, L__64 op_count, L__64 op_pages) {
    if (ctx->functions_count >= ctx->functions_page_count * ctx->functions_page_size) {
        void* temp;
        if ((temp = LSTC_simplify_mmap_private((ctx->functions_page_count + 4) * sizeof(void*)))) {
            return 0;
        }
        L__64 old = ctx->functions_page_count -1;
        LSTC_memcpySD(temp, ctx->functions, ctx->functions_page_count * sizeof(void*));
        LSTC_munmap(ctx->functions, ctx->functions_page_count * sizeof(void*));
        ctx->functions = temp;
        ctx->functions_page_count+= 4;
        for (int i = 0; )
        return 0;
    }
    LSTC_asmgen_indx pgin = LSTC_ASMGEN_helperIndx_fn(ctx, ctx->functions_count);
    LSTC_asm_function* fn = &ctx->functions[pgin.PAGE][pgin.INDX];

    fn->name = name;
    fn->name_sz = 0;
    while (name[fn->name_sz]) fn->name_sz++;
    fn->instruction_count = 0;
    fn->operands_pages = ctx->operands_pages;
    fn->op_pagec = op_pages;

    ctx->functions_count++;
    return fn;
}
static void* LSTC_asm_create_operands_page() {return 0;}
static inline void LSTC_asm_set_entry(LSTC_asm_container* self, LSTC_asm_function* fn) {self->entry = fn;} // for future logic expansion
static inline void LSTC_asm_set_name(LSTC_asm_container* self, char* name) {self->app_name = name;} // same

#endif
