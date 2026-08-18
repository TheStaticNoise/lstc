#ifndef LSTC_UNORDERED_MAP
#define LSTC_UNORDERED_MAP

#include "LSTC_primitives.h"
#include "LSTC_pager.h"

#define LSTC_UMAP_GROWTH_COEFFICIENT(e) (e) = ((e) >> 2) + (e / 10)

typedef enum {
    INT,
    CHAR,
    STRING,
    PTR,
    FLOAT,
    PRIVATE // unknown struct (for the hashing algorithm)
} LSTC_UMAP_TYPES;

/*
 * Hashing:
 * int - just value (BYPASS)
 * char - just promoted value (BYPASS)
 * ptr - NO HASHING
 * private - dereference & n = value_size
 */

typedef struct {
    void* bucket;
    long    size; // Power of 2
    long unit_size;
    long value_size;
    LSTC_UMAP_TYPES key_type; // how to hash
    LSTC_UMAP_TYPES value_type; // how to interpret
} LSTC_umap;

typedef struct {
    void* value; // baked in if smaller
    L__U64 size;
} LSTC_BUCKET_ELEMENT;

typedef struct {
    void**    bucket; // array of pointers (paged!)
    L__U64     pagec;
    L__U64     usize; // unit size
    L__U64     bsize; // Power of 2, size of one page bucket
    LSTC_UMAP_TYPES type;
    L__U64 global_error; // checked on every op
    L__U8 lock; //! 0 = unlocked, byte 0 = read only
} LSTC_umap_string;

LSTC_umap_string LSTC_create_UMAP_string(L__U64 bsize /* ! */, L__U64 init_pages, L__U64 usize,  LSTC_UMAP_TYPES endType) {
    // identity size x = n^2, endType! unitsz! (0 if inrefer!)
    // global error list
    // 1 : unnable to inrefer type
    // 2 : mmap fail stage 1 (alloc ptr array!)
    // 3 : mmap fail stage 2 (alloc page!)

    LSTC_umap_string ident = {0}; //!
    if (usize == 0) {
        switch (endType) {
            // lstc primitives (the backbone of lstcClibs) expect those sizes for ints:
            // short int = 2
            // int = 4
            // long long int = 8
            // so, we expect a x86_64 architecture, so we can safely assume sizes.
            case STRING:
            case PTR:
                usize = 8; // copy, can be safely edited.
            case INT:
            case FLOAT:
                usize = 4; // both have same size
            case CHAR:
                usize = 1;
            case PRIVATE:
                ident.global_error = 1;
                return ident;
        }
    }
    void* e = LSTC_simplify_mmap_private((init_pages + (init_pages >> 2 > 10 ? 10 : init_pages >> 2) + (init_pages >> 4)) * 8);

}

#define LSTC_CREATE_STR_UMAP(type1, type2)
#endif
