#ifndef PRINTF_DEFS
#define PRINTF_DEFS

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"

int printf(const char *format, ...);
int snprintf(char *buf, int n, const char *fmt, ...);

void puts(const char *msg);

void putchar(int c);
void uart_init ( void );

int strcmp(const char *_p, const char *q);
void *memset(void *_p, int c, unsigned n) ;
void reboot(void) __attribute__((noreturn));

#endif
