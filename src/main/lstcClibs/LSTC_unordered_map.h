#ifndef LSTC_UNORDERED_MAP
#define LSTC_UNORDERED_MAP

#include "LSTC_primitives.h"


static inline L__U64 fnv1a64(const char* buf, L__U64 len) {
	L__U64 out = 0xcbf29ce484222325;

	for (L__U64 i = 0; i < len; i++)
		out = (out ^ buf[i]) * 0x00000100000001b3;

	return out;
}


typedef enum {
    INT,
    CHAR,
    STRING
} LSTC_UMAP_TYPES;

typedef struct {
    void* bucket;
    long    size; // Power of 2
    long unit_size;
    LSTC_UMAP_TYPES key_type; // how to hash
    LSTC_UMAP_TYPES value_type;
} LSTC_umap;

#define LSTC_CREATE_UMAP(type1, type2);
#endif
