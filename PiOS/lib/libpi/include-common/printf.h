#ifndef PRINTF_DEFS
#define PRINTF_DEFS

#include <integer.h>

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wpragmas"
//#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"

int printf(const char *format, ...);
int snprintf(char *buf, size_t n, const char *fmt, ...);
int puts(const char *msg);
int putchar(int c);

int strcmp(const char *_p, const char *q);
void *memset(void *_p, int c, unsigned n) ;
void reboot(void) __attribute__((noreturn));

#endif
