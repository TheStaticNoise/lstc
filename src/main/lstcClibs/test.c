#include "LSTC_primitives.h"
#include "LSTC_pager.h"
#include "LSTC_io.h"
#include "LSTC_exit.h"

int main(void) {
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
    LSTC_exit(0);
}
