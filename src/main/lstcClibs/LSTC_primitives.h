#ifndef LSTC_PRIMITIVES
#define LSTC_PRIMITIVES


#if '\n' != 0x0A
#error "stinky EBCDIC mainframes are not supported. ASCII required bitch!" // EBCDICK
#endif

typedef signed char L__S8;
typedef short int  L__S16;
typedef int        L__S32;
typedef long long  L__S64;

_Static_assert(sizeof(L__S8)  == 1 &&
    sizeof(L__S16) == 2 &&
    sizeof(L__S32) == 4 &&
    sizeof(L__S64) == 8,
    "Stinky system detected");

typedef unsigned char       L__U8;
typedef unsigned short int  L__U16;
typedef unsigned int        L__U32;
typedef unsigned long long  L__U64;

#define L__BOOL  unsigned char
#define L__TRUE  1
#define L__FALSE 0
#define L__NULL 0

typedef enum {
    LSTC_INT,
    LSTC_STRUCT,
    LSTC_STRING,
    LSTC_CHAR,
    LSTC_FLOAT,
    LSTC_DOUBLE,
} types;

static inline L__U32 LSTCPr_fnv1a(const L__U8 *buf, L__U32 len) {
	L__U32 out = 0x811c9dc5;

	for (L__U32 i = 0; i < len; i++)
		out = (out ^ buf[i]) * 0x01000193;

	return out;
}

static inline L__BOOL LSTCPr_compN_mem(const L__U8* b1, const L__U8* b2, L__U64 n) {
    for (L__U64 e = 0; e < n; e++) {
        if (*b1 == *b2) {
            b1++, b2++;
        } else {
            return L__FALSE;
        }
    }
    return L__TRUE;
}

static inline L__BOOL LSTCRr_CompNf(char *b1, char *b2, L__U64 n) {
    L__U64 *acc = (L__U64*)b1, *acc2 = (L__U64*)b2;
    int indx = 0;
    for (L__U64 e = n; e != 0;) {
        if (e >= 8) {
            if (acc[indx] == acc2[indx]) { // NO Need for cast here
                e -= 8;
                indx+=8;
                continue;
            }
            return L__FALSE;
        }
        if (e >= 4) {
            if ((L__U32) acc[indx] == (L__U32) acc2[indx]) {
                e -= 4;
                indx+= 4;
                continue;
            }
            return L__FALSE;
        }
        if (e >= 2) {
            if ((L__U16) acc[indx] == (L__U16) acc2[indx]) {
                e -= 2;
                continue;
            }
            return L__FALSE;
        }
        if (e >= 1) {
            if ((L__U8) *b1 == (L__U8) *b2) {
                b1 += 1; b2 += 1;
                e -= 1;
                continue;
            }
            return L__FALSE;
        }
    }
    return L__TRUE;
}

#endif
