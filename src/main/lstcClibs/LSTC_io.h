#ifndef LSTC_IO
#define LSTC_IO
#include "LSTC_primitives.h"
typedef unsigned short LSTC_LinuxUMode_t;
extern L__S64 LSTC_write(int fd, const char* buf, L__U64 ctn);
extern L__S64 LSTC_read(int fd, char* buf, L__U64 ctn);
extern L__S64   LSTC_open(const char* name, int flags, LSTC_LinuxUMode_t mode);
extern L__S64   LSTC_close(int fd);
#endif
