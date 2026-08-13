#include "LSTC_primitives.h"
#include "LSTC_pager.h"
#include "LSTC_io.h"
#include "LSTC_exit.h"
#include "LSTC_xtoy.h"
#include "LSTC_getparam.h"

int LSTC_main(L__U64 rsp_ptr) /* CAN YOU DO THIS IN LIBC C CODE??? */ {
    LSTC_write(1, "test: ", 7);
    char bufff[16];
    LSTC_params e;
    e = LSTC_GetParams(rsp_ptr);
    unsigned char r = LSTCPr_compN_mem("Exactly!", "Exactly!", 9);
    r+= '0';
    LSTC_write(1, &r, 1);
    LSTC_write(1, "\n", 1);
    return 0;
}
