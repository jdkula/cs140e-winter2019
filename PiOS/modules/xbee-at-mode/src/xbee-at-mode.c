
#include <rpi.h>
#include <uart.h>
#include <aux.h>
#include <mem-barrier.h>
#include <timer.h>
#include <printf.h>
#include <gpio.h>
#include <arm/stdlib.h>
#include <softuart.h>
#include <interrupts.h>
#include <xbee.h>

#define LED_PIN 13
#define TXPIN 21
#define RXPIN 20
#define BUFSIZE 128

#define RESETPIN 5

int notmain(void) {
    gpio_init();
    gpio_set_output(LED_PIN);

    gpio_set_output(RESETPIN);
    gpio_write(RESETPIN, HIGH);

    soft_uart_init_timer_helper(SOFT_UART_9600);

    initialize_interrupts(RPI_BASIC_ARM_TIMER_IRQ, 0, RPI_INTREG2_GPIO_ALL_IRQ);

    soft_uart_init(TXPIN, RXPIN, SOFT_UART_9600);

    uart_init();

    xbee_init(RESETPIN);

    delay_ms(10000);

    char buff[1024];

    soft_uart_puts("About to get address info...\r\n");

    printk("TEST");

    struct xbee_addresses info = xbee_get_address_info();

    snprintk(buff, 5000, "SN: %x | %x\r\n", info.serial_num_high, info.serial_num_low);
    soft_uart_puts(buff);

    snprintk(buff, 5000, "Device Type: %x\r\n", info.device_type_identifier);
    soft_uart_puts(buff);

    while(true) {}
}