#include "LSTC_primitives.h"
#include "LSTC_pager.h"
#include "LSTC_io.h"
#include "LSTC_exit.h"
#include "LSTC_xtoy.h"
#include "LSTC_getparam.h"

int LSTC_main(L__U64 rsp_ptr) /* CAN YOU DO THIS IN LIBC C CODE??? */ {
    LSTC_write(1, "Param ctn: ", 11);
    char bufff[16];
    LSTC_params e;
    e = LSTC_GetParams(rsp_ptr);
    LSTC_itoa(-2147483648, bufff, 16);
    LSTC_write(1, bufff, 16);
    LSTC_write(1, "\n", 1);
    return 0;
}
