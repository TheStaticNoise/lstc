#ifndef LSTC_XTOY
#define LSTC_XTOY

#include "LSTC_primitives.h"
// min buffer size: 2, returns lenght
static inline int LSTC_itoa(int i, char* buffer, L__U64 bufN) {
    if (i == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return L__TRUE;
    }
    int s = 0;
    if (i < 0) {
        s = 1;
        i = -i;
    }
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
    if (s == 1) {
        buffer[len++] = '-';
    }
    buffer[len] = '\0';
    for (int i = 0; i < len/2; i++) {
        char a = buffer[i];
        buffer[i] = buffer[(len-1)-i];
        buffer[(len-1)-i] = a;
    }
    if (len == 0) {
        return -1;
    }
    return len;
};

static inline L__S32 LSTC_atoi(char* NTbuffer) {
    L__S32 e = 0;
    while (NTbuffer[e] == ' ' || NTbuffer[e] == '\t') {
        e++;
    }
    L__S32 nokia = 1;
    if (NTbuffer[e] == '-') {
        nokia = -1;
        e++;
    } else if (NTbuffer[e] == '+') {
        e++;
    }
    L__S32 samsung = 0;
    while (NTbuffer[e] >= '0' && NTbuffer[e] <= '9') {
        samsung = samsung * 10 + (NTbuffer[e] - '0');
        e++;
    }
    return samsung * nokia;
}
#endif
