
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


int notmain(void) {
    gpio_init();
    gpio_set_output(GPIO_ACT);
    gpio_write(GPIO_ACT, LOW);

    gpio_set_output(RESETPIN);
    gpio_write(RESETPIN, HIGH);

    gpio_set_output(26);
    gpio_write(26, LOW);

    uart_init();

    struct xbee_addresses info = xbee_get_address_info();

    while(true) {
        char c = uart_getc();
        uart_putc(c);
        if(c == 'o') {
            gpio_write(26, HIGH);
        } else if (c == 'f'){
            gpio_write(26, LOW);
        }

        // note - the functionality for this if statement and below was commented out during the presentation.
        // I was really setting up that presentation last minute and didn't have time to give the below a good test.
        if (c == 'I') {
            uart_putc('I');
            printk("\r\nREMOTE:\r\n");
            char buf[256];
            snprintk(buf, 256, "SN: %x%x\r\n", info.serial_num_high, info.serial_num_low);
            printk(buf);
            snprintk(buf, 256, "Dest: %x%x\r\n", info.destination_high, info.destination_low);
            printk(buf);
            snprintk(buf, 256, "Address: %x\r\n", info.network_address);
            printk(buf);
        } else if (c == 's') {
            uart_putc('s');

            gpio_write(GPIO_ACT, HIGH);

            uint32_t time = timer_get_time();
            uint32_t max_time = 30000000;

            char inbuf[256];
            uint32_t inbuf_i = 0;

            for (uint32_t i = 0; i < NUM_TIMES; i++) {
                for (char l = 'A'; l < 'Z'; l++) {
                    while (!aux_uart_rx_has_data()) {
                        if (timer_get_time() > (time + max_time)) {
                            goto end;
                        }
                    }
                    char c = uart_getc();
                    inbuf[inbuf_i++] = c;
                }
            }

            end:
            gpio_write(GPIO_ACT, LOW);

            uint32_t done = timer_get_time();

            for (uint32_t i = 0; i < inbuf_i; i++) {
                uart_putc(inbuf[i]);
            }

            char buf[256];
            snprintk(buf, 256, "\r\nTook %d ticks; got %d bytes (of %d bytes).\r\n", done - time, inbuf_i,
                     NUM_TIMES * ('Z' - 'A' + 1));
            soft_uart_puts(buf);
            printk(buf);
        }
    }
}