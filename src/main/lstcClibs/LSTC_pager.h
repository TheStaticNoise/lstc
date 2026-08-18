#ifndef LSTC_PAGER
#define LSTC_PAGER

#include "LSTC_primitives.h"

#define LSTC_PAGER_MMAP_SYSC 9
#define LSTC_PAGER_MUNMAP_SYSC 11
#define LSTC_PAGER_MMAP_READ 1
#define LSTC_PAGER_MMAP_WRITE 2
#define LSTC_PAGER_MMAP_ANONFL 32
#define LSTC_PAGER_MMAP_PRIVATE 2
#define LSTC_PAGER_MMAP_ANONFD -1
#define LSTC_PAGER_EXTRACAP 16

typedef struct {
    void**          data;
    L__U64         chunk;
    L__U64         pages;
    L__U64     max_pages;
    L__U64 chunks_inited;
} LSTC_PAGER_context;


// provides you an good starting allocation and shit
[[nodiscard]]
void* LSTC_mmap(void* address, L__U64 len, int p, int fl, int fd, L__U64 off);
[[nodiscard]]
void* LSTC_munmap(void* address, L__U64 len); // both are in MMAP.s
[[nodiscard]]
LSTC_PAGER_context LSTC_PAGER_init(L__U64 chunk_size, L__U16 size);
[[nodiscard]]
L__BOOL LSTC_PAGER_mun(LSTC_PAGER_context* e);
[[nodiscard]]
static inline void* LSTC_simplify_mmap_private(L__U64 size) {
    void* xd = LSTC_mmap(0, size, LSTC_PAGER_MMAP_WRITE | LSTC_PAGER_MMAP_READ, 34, -1, 0);
    return (L__U64) xd > (L__U64) -4096 ? 0 : xd;
}

#define LSTC_MPINIT(chunk_size, init_size) LSTC_PAGER_init(chunk_size, init_size)
#define LSTC_MPKILL(context) LSTC_PAGER_mun(context)
#endif
