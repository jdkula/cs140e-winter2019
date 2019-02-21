#include <uart.h>
#include <printf.h>

int notmain(void) {
    uart_init();

#if 0
    uart_putc('h');
	uart_putc('e');
	uart_putc('l');
	uart_putc('l');
	uart_putc('o');

	uart_putc(' ');

	uart_putc('w');
	uart_putc('o');
	uart_putc('r');
	uart_putc('l');
	uart_putc('d');

	uart_putc('\n');
#endif

    printk("Hello World!\n");

    return 0;
}