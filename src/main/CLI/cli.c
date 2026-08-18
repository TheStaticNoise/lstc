#include "../lstcClibs/LSTC_primitives.h"
#include "../lstcClibs/LSTC_getparam.h"
#include "../lstcClibs/LSTC_pager.h"
#include "../lstcClibs/LSTC_xtoy.h"
#include "../lstcClibs/LSTC_io.h"

// req: LSTC C LIBS start.s / start.o

#define NULL 0
#define BM_HAS_INPUT  1
#define BM_HAS_OUT    2
#define BM_VERSION    4
#define BM_HELP       8
#define BM_DEBUG     16

// welcome to one of the most insane codebases

L__U64 Param_patternMatch(void* page, int argc, char** argv, void* buf, int n) {

    return 0;
}

L__U64 LSTC_main(L__U64 rsp_ptr) {
    LSTC_params e = LSTC_GetParams(rsp_ptr);
    char earr[16];
    L__U64 r = Param_patternMatch(NULL, e.argc, e.argv, earr, 16);
}
