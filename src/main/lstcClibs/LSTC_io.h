#ifndef LSTC_IO
#define LSTC_IO
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
#endif
