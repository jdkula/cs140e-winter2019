
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

    delay_ms(5000);

//    soft_uart_send_now("Soft UART initialized.\r\n", 24);


    uart_init();

    gpio_write(LED_PIN, HIGH);
    delay_ms(200);
    gpio_write(LED_PIN, LOW);

//    soft_uart_send_now("About to send control sequence.\r\n", 33);

//    gpio_set_function(GPIO_TX, GPIO_FUNC_OUTPUT);

    gpio_write(LED_PIN, HIGH);
//    gpio_write(GPIO_TX, LOW);
    gpio_write(RESETPIN, LOW);
    delay_ms(500);
    gpio_write(RESETPIN, HIGH);
    gpio_write(LED_PIN, LOW);
//    gpio_write(GPIO_TX, HIGH);

//    gpio_set_function(GPIO_TX, GPIO_FUNC_UART); // BCM2835AP §6.2 p. 102


    delay_ms(2000);
    uart_putc('+');
    delay_ms(100);
    uart_putc('+');
    delay_ms(100);
    uart_putc('+');
    delay_ms(100);

//    soft_uart_send_now("Sent.\r\n", 7);

    char c;

    c = uart_getc();  // O
    soft_uart_putc(c);
    c = uart_getc();  // K
    soft_uart_putc(c);
    c = uart_getc();  // \r
    soft_uart_putc(c);
//    soft_uart_send_now("\r\n", 2);

    gpio_write(LED_PIN, HIGH);
    delay_ms(200);
    gpio_write(LED_PIN, LOW);

//    soft_uart_send_now("Getting Serial.\r\n", 17);

    printk("ATSH\r");

    char out[7];

    for(int i = 0; i < 7; i++) {
        out[i] = uart_getc();
    }
    out[6] = '\0';

//    soft_uart_send_now(out, 7);

    if(strncmp(out, "13A200", 6) == 0) {
        gpio_write(LED_PIN, HIGH);
    }

    while(1);

    reboot();
    return 0;
}