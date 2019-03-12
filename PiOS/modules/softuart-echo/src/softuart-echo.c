
#include <softuart.h>
#include <boolean.h>
#include <timer.h>
#include <gpio.h>
#include <rpi.h>
#include <interrupts.h>
#include <uart.h>
#include <printf.h>
#include <aux.h>


void notmain() {
    uart_init();
    gpio_init();

    soft_uart_init_timer_helper(SOFT_UART_9600);

    initialize_interrupts(RPI_BASIC_ARM_TIMER_IRQ, 0, RPI_INTREG2_GPIO_ALL_IRQ);

    soft_uart_init(21, 20, SOFT_UART_9600);

    while(true) {
        char c = soft_uart_getc();
        soft_uart_putc(c);
        printk("Status: %d%d%d%d\r\nReceived: %c (%d) (0b%d%d%d%d%d%d%d%d)",
               soft_uart_can_transmit(),
               soft_uart_can_receive(),
               soft_uart_should_transmit(),
               soft_uart_has_characters(),
               c,
               c,
               (c >> 0) & 1,
               (c >> 1) & 1,
               (c >> 2) & 1,
               (c >> 3) & 1,
               (c >> 4) & 1,
               (c >> 5) & 1,
               (c >> 6) & 1,
               (c >> 7) & 1);
    }

    reboot();
}