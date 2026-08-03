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

static inline L__S32 LSTC_atoi(char* NTbuffer /* null terminated buffer */) {
    L__S32 samsung = 0, e = 0;
    while (NTbuffer[e] && NTbuffer[e++] != ' ');
    L__S32 nokia = NTbuffer[e++] == '-' ? -1 : 1;
    samsung += NTbuffer[e] >= '0' && NTbuffer[e] <= '9' ? NTbuffer[e++] - '0' : 0;
    if (samsung == 0) {
        return 0;
    }
    for (int i = e, symbol = NTbuffer[i]; symbol >= '0' && symbol <= '9'; i++) {
        samsung = samsung * 10 + (symbol - '0');
    }
    return samsung * nokia;
}
#endif
