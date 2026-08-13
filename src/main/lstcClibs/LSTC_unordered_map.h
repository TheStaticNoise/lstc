#ifndef LSTC_UNORDERED_MAP
#define LSTC_UNORDERED_MAP

#include "LSTC_primitives.h"


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
    LSTC_UMAP_TYPES value_type; // how to interpret
} LSTC_umap;

typedef struct {
    void** bucket; // array of pointers
    long    size; // Power of 2
    long unit_size;
    LSTC_UMAP_TYPES value_type; // how to interpret
} LSTC_umap_string;



#define LSTC_CREATE_STR_UMAP(type1, type2);
#endif
