#ifndef LSTC_PRIMITIVES
#define LSTC_PRIMITIVES

#if '\n' != 0x0A
#error "stinky EBCDIC mainframes are not supported. ASCII required bitch!" // EBCDICK
// deleting this check won't help, the entire user facing architecture assumes ASCII
// P.S. that's like removing the warning sticker "Toxic!" and thinking its now safe
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

#define L__8 L__U8
#define L__16 L__U16
#define L__32 L__U32
#define L__64 L__U64

#define LSTC_IS_LETTER(e) (((e) >= 'A' && (e) <= 'Z') || ((e) >= 'a' && (e) <= 'z'))
#define LSTC_SWITCH_CASE(e) do { if ASCII_IS_LETTER(e) { e ^= 0x20; } } while (0)

#define LSTC_ABS(exp) (((exp) < 0) ? -(exp) : (exp))

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
static inline L__U32 LSTCPr_fnv1aF(const L__U8 *buffer) {
    L__U64 i = 0;
    for (; buffer[i] != '\0'; i++);
	return LSTCPr_fnv1a(buffer, i);
}


static inline L__BOOL LSTCPr_compN_mem(const L__8* b1, const L__8* b2, L__64 n) {
    for (L__64 e = 0; e < n; e++) {
        if (*b1 == *b2) {
            b1++, b2++;
        } else {
            return L__FALSE;
        }
    }
    return L__TRUE;
}

static inline void LSTC_memcpy(L__8* b1, L__8* b2, L__64 n) {
    for (L__64 i = 0; i < n; i++) {
        b2[i] = b1[i];
    }
}

#endif
