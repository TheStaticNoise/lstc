#include "LSTC_pager.h"
#include "LSTC_primitives.h"

LSTC_PAGER_context LSTC_PAGER_init(L__U64 chunk_size, L__U16 size) {
    LSTC_PAGER_context e = {.chunks_inited = 0, .chunk = chunk_size, .pages = size};
    if (size == 0) {
        return e;
    }
    e.max_pages = size + LSTC_PAGER_EXTRACAP;
    void *ptr1 = LSTC_mmap(0, sizeof(void*) * e.max_pages, LSTC_PAGER_MMAP_WRITE | LSTC_PAGER_MMAP_READ, 34, -1, 0);
    e.data = ptr1;
    for (int i = 0; i < size; i++) {
        void *ptr2 = LSTC_mmap(0, chunk_size, LSTC_PAGER_MMAP_WRITE | LSTC_PAGER_MMAP_READ, 34, -1, 0);
        if (ptr2 >= -4096) {
            break;
        }
        e.data[i] = ptr2;
        e.chunks_inited++;
    }
    return e;
}

L__BOOL LSTC_PAGER_grow(LSTC_PAGER_context* e, L__U64 n) {
    void** tmp;
    if (e->max_pages < e->pages + n) {
        tmp = LSTC_mmap(0, (e->pages + n + LSTC_PAGER_EXTRACAP) * sizeof(void*), 3, 34, -1, 0);
        if (tmp >= -4096) {
            return L__FALSE;
        }
    }
    for (int i = 0; i < e->chunks_inited; i++) {
        tmp[i] = e->data[i];
    }
    e->data = tmp;
    L__U64 ea = n;
    while (ea != 0) {
        e->data[e->chunks_inited] = LSTC_mmap(0, e->chunk, 3, 34, -1, 0);
        if (e->data[e->chunks_inited] >= -4096) {
            return L__FALSE;
        }
        e->chunks_inited++;
        ea--;
    }
    return L__TRUE;
}

L__BOOL LSTC_PAGER_mun(LSTC_PAGER_context* e) {
    while (e->chunks_inited != 0) {
        if (LSTC_munmap(e->data[e->chunks_inited - 1], e->chunk) == -1) {
            return L__FALSE;
        } e->chunks_inited--;
    }
    return L__TRUE;
}
