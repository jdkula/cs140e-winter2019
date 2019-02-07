#ifndef PRINTF_DEFS
#define PRINTF_DEFS

#include <integer.h>

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wpragmas"
//#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"

int printk(const char *format, ...);
int snprintk(char *buf, size_t n, const char *fmt, ...);
int putk(const char *msg);

#endif
