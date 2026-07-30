#include "LSTC_primitives.h"
#include "LSTC_pager.h"
#include "LSTC_io.h"
#include "LSTC_exit.h"
#include "LSTC_xtoy.h"

int main(L__U64 rsp_ptr) /* CAN YOU DO THIS IN LIBC C CODE??? */ {
    LSTC_PAGER_context pag = LSTC_MPINIT(4096, 6);
    if (pag.chunks_inited != pag.pages) {
        LSTC_write(1, "failed to alloc", sizeof("failed to alloc"));
        LSTC_exit(-1);
    }
    LSTC_write(1, "XD\n", 3);
    // now we own 6 pages each 4096, aka 24 KB total
    if (!LSTC_MPKILL(&pag)) {
        LSTC_write(1, "U are an idiot sandwich", sizeof("U are an idiot sandwich"));
        LSTC_exit(-1);
    };
    char bufff[16];
    LSTC_itoa(600613, bufff, 16);
    LSTC_write(1, bufff, 16);
    LSTC_write(1, " sucks \n",  8);
    LSTC_exit(0);
}
