#ifndef LSTC_GETPARAMS
#define LSTC_GETPARAMS

#include "LSTC_primitives.h"
typedef struct {
    L__U64 rsp;
    L__U64 argc;
    char** argv;
    L__U64 envc;
    char** envp;
} LSTC_params;

static LSTC_params LSTC_GetParams(L__U64 rsp) {
    LSTC_params e;
    e.rsp = rsp;
    e.argc = *((L__U64*) rsp);
    e.argv = (char**)(rsp + 8);
    e.envp = (char**)(rsp + 8 + (e.argc + 1) * 8);
    for (e.envc = 0; e.envp[e.envc]; e.envc++);
    return e;
}

#endif
