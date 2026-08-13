#ifndef LSTC_vector
#define LSTC_vector



#include "LSTC_pager.h"
#include "LSTC_primitives.h"

typedef struct {
    void** storage;
    L__U64 MMAP_sz; // 16192 (16kb) std
    L__U64 pgcount; // page count
    L__U64    size; // size per page
    L__U64    curr; // current index
    L__U8      err;
} LSTC_vec;


static LSTC_vec LSTC_VEC_init(L__U64 n, L__U64 mmaps, L__U64 initpgc) {
    LSTC_vec e = {.err = 0, .pgcount = 0, .size = n, .curr = 0};
    void** t = LSTC_mmap(0, initpgc * sizeof(void*), 3, 34, -1, 0);
    if (t >= -4096LL) {e.err = 1; return e;}
    e.storage = t;
    e.MMAP_sz = mmaps == 0 ? 16192 : mmaps;
    for (L__U64 i = 0; i < initpgc; i++, e.pgcount++) {
        L__U64* r = LSTC_mmap(0, e.MMAP_sz, 3, 34, -1, 0);
        if (r >= -4096LL) {e.err = 1; return e;} // love abusing 2's compliment
        t[i] = r; }
    return e;}

static inline void LSTC_VEC_destruct(LSTC_vec vecc) {
    for (int i = 0; i < vecc.pgcount; i++) LSTC_munmap(vecc.storage[i], vecc.MMAP_sz);
}

#endif
