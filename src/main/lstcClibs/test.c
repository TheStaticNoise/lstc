#include "LSTC_primitives.h"
#include "LSTC_pager.h"
#include "LSTC_io.h"
#include "LSTC_exit.h"
#include "LSTC_xtoy.h"
#include "LSTC_getparam.h"

void test_buffered_out() {
    char e[9] = "HEADER";
    LSTC_IO_BUFFER_Ctx r = LSTC_create_ctx(1, 6);
    for (int i = 6; i < 8; i++) {
        e[i]='a';
    }
    e[7] = '\n';
    e[8] = '\0';
    LSTC_printb(e, &r);
    LSTC_flushctx(&r);
    LSTC_printb("HEADERXD\n", &r);
    LSTC_printb("YAY\n", &r);
    LSTC_flushctx(&r);
    LSTC_destructctx(&r);
    return;
}

int testABS(int r) {
    char e[16];
    LSTC_itoa(LSTC_ABS(r), e, 16);
    LSTC_print(e, -1);
    return LSTC_ABS(r);
}

void test_xtoy_int() {
    int a = 43;
    char r[16];
    LSTC_itoa(a, r, 16);
    a = LSTC_atoi(r);
    if (a == 43) {
        LSTC_print("V\n", -1);
    } else {
        LSTC_itoa(a, r, 16);
        LSTC_print(r, -1);
        LSTC_print(" - X\n", -1);
    }
    return;
}

int LSTC_main(L__U64 rsp_ptr) /* CAN YOU DO THIS IN LIBC C CODE??? */ {
    if (testABS(-8) == 8) {
        LSTC_print(" - V\n", -1);
    };
    test_buffered_out();
    test_xtoy_int();
    return 0;
}
