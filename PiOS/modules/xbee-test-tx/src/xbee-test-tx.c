
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

#define NUM_TIMES 10

#define CTS_PIN 8

uint8_t START = 's';


bool pause_sending = false;

bool initialized = false;

void cts() {
    if(gpio_check_and_clear_event(CTS_PIN)) {
        soft_uart_puts("GOT CTS INTERRUPT! ");
        if (gpio_read(CTS_PIN) == LOW) {
            soft_uart_puts("Restarting.\r\n");
            pause_sending = false;
        } else {
            soft_uart_puts("Stopping.\r\n");
            pause_sending = true;
        }
    }
}


int notmain(void) {
    if(!initialized) {
        gpio_init();
        gpio_set_output(LED_PIN);

        gpio_set_output(RESETPIN);
        gpio_write(RESETPIN, HIGH);

        soft_uart_init_timer_helper(SOFT_UART_9600);

        initialize_interrupts(RPI_BASIC_ARM_TIMER_IRQ, 0, RPI_INTREG2_GPIO_ALL_IRQ);

        soft_uart_init(TXPIN, RXPIN, SOFT_UART_9600);

        uart_init();
        initialized = true;
    }

    char buf[64];
    uint32_t max_time = 1000000;
    while(true) {
        char c = soft_uart_getc();
        snprintk(buf, 64, "\r\n>> Got %c\r\n<< ", c);
        soft_uart_puts(buf);
        uart_putc(c);
        uint32_t time = timer_get_time();
        while(!aux_uart_rx_has_data() && timer_get_time() < (time + max_time)) ;
        if(timer_get_time() >= (time + max_time)) continue;
        soft_uart_putc(uart_getc());
    }
}