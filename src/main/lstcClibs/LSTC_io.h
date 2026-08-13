#ifndef LSTC_IO
#define LSTC_IO

#include "LSTC_pager.h"
#include "LSTC_primitives.h"

#define LSTC_OPEN_MODE_RWE_R 0755
#define LSTC_OPEN_MODE_RW_X  0600
#define LSTC_OPEN_MODE_RW_R  0644

#define LSTC_O_RDONLY  0x0000
#define LSTC_O_WRONLY  0x0001
#define LSTC_O_RDWR    0x0002
#define LSTC_O_CREAT   0x0040
#define LSTC_O_TRUNC   0x0200
#define LSTC_O_APPEND  0x0400

#define LSTC_O_RELATIVE -100

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

typedef unsigned short LSTC_LinuxUMode_t;

typedef struct {
    int fd;
    int readPtr;
    int permissions;
} LSTC_IO_streamDesc;

typedef struct {
    char* buffer_page; // support only 1 buffer page
    L__U64 index;
    L__U64 outsz;
    L__U64 chunk;
    int flush_dest; // !
    // buffer page, index,
} LSTC_IO_BUFFER_Ctx;

extern L__S64 LSTC_write(int fd, const char* buf, L__U64 ctn);
extern L__S64 LSTC_read(int fd, char* buf, L__U64 ctn);
extern L__S64 LSTC_open(int dirfd, const char* name, int flags, LSTC_LinuxUMode_t mode) ;
extern L__S64 LSTC_close(int fd);
extern L__S64 LSTC_lseek(int fd, L__S64 offset, L__U32 whence);
extern L__S64 LSTC_create(const char* name, LSTC_LinuxUMode_t mode);

static L__BOOL LSTC_print(const char* o, int fd) { // -1 if std
    int out = 1;
    if (fd != -1) {
        out = fd;
    }
    const char* res = o;
    while (*res++ != 0);
    L__U64 size = res - o;
    for (L__S64 i = 0, e; i < size;) {
        e = LSTC_write(out, &o[i], size - i);
        if (e < 0) {
            return L__FALSE;
        }
        i += e;
    }
    return L__TRUE;
}

static L__BOOL LSTC_printEXP(const char* o, int fd, L__U64 sz) { // -1 if std
    int out = 1;
    if (fd != -1) {
        out = fd;
    }
    for (L__S64 i = 0, e; i < sz;) {
        e = LSTC_write(out, &o[i], sz - i);
        if (e < 0) {
            return L__FALSE;
        }
        i += e;
    }
    return L__TRUE;
}

static inline LSTC_IO_BUFFER_Ctx LSTC_create_ctx(int fd, L__U64 sz) {
    LSTC_IO_BUFFER_Ctx e = {
        .buffer_page = 0,
        .index = 0,
        .outsz = 0,
        .chunk = sz == -1 ? 4096 : sz,
        .flush_dest = fd,
    };
    char* temp = (char*) LSTC_mmap(0, e.chunk, 3, 34, -1, 0);

    e.buffer_page = temp > 4096ull ? 0 : temp;

    return e;
}

static inline void LSTC_flushctx(LSTC_IO_BUFFER_Ctx* ctx) {
    LSTC_printEXP(ctx->buffer_page, ctx->flush_dest, ctx->outsz);
    ctx->outsz = 0;
    ctx->index = 0;
}

static L__BOOL LSTC_printb(const char* o, LSTC_IO_BUFFER_Ctx ctx);

#endif
