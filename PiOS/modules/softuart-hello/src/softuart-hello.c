
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

//    soft_uart_init_timer_helper(SOFT_UART_9600);

//    initialize_interrupts(RPI_BASIC_ARM_TIMER_IRQ, 0, RPI_INTREG2_GPIO_ALL_IRQ);

    soft_uart_init(21, 20, SOFT_UART_9600);

    while(true) {
//        while(!soft_uart_can_transmit());
        soft_uart_puts("Hello World!\r\n");
        printk("Hello World!\r\n");
        delay_ms(1000);
    }

    reboot();
}