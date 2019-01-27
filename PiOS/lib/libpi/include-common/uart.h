#ifndef UART_DEFS
#define UART_DEFS

#include <integer.h>

void uart_init(void);

int uart_getc(void);

void uart_putc(uint32 c);

#endif
