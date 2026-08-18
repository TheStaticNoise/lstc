#include "LSTC_primitives.h"
#include "LSTC_pager.h"
#include "LSTC_io.h"
#include "LSTC_exit.h"
#include "LSTC_xtoy.h"
#include "LSTC_getparam.h"

void test_buffered_out() {
    char e[67] = "HEADER";
    LSTC_IO_BUFFER_Ctx r = LSTC_create_ctx(1, 64);
    for (int i = 6; i < 64; i++) {
        e[i]='a';
    }
    e[64] = 'e';
    e[65] = '\n';
    e[66] = '\0';
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
    LSTC_print("\n", -1);
}

int LSTC_main(L__U64 rsp_ptr) /* CAN YOU DO THIS IN LIBC C CODE??? */ {
    testABS(-8);
    test_buffered_out();
}
