#ifndef LSTC_XTOY
#define LSTC_XTOY

#include "LSTC_primitives.h"


static inline L__BOOL LSTC_itoa(int i, char* buffer, L__U64 bufN) {
    int len = 0;
    for (int mirror = i; mirror != 0 && len < bufN-1;) {
        int buf = mirror, e;
        if (mirror < 10) {
            buffer[len++] = '0' + mirror;
            break;
        }
        buf = mirror / 10;
        e = mirror - (buf*10);
        buffer[len++] = '0' + e;
        mirror = buf;
    }
    buffer[len] = '\0';
    for (int i = 0; i < (len)/2; i++) {
        char a = buffer[i];
        buffer[i] = buffer[(len-1)-i];
        buffer[(len-1)-i] = a;
    }
    if (len == 0) {
        return L__FALSE;
    }
    return L__TRUE;
}


#endif
