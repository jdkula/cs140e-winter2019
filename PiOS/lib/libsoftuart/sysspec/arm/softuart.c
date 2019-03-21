//
// Created by jonak on 3/10/19.
//

#include <gpio.h>
#include <timer.h>
#include <armtimer.h>
#include <interrupts.h>
#include <printf.h>
#include "softuart.h"


struct softuart softuart_current = {0};

bool soft_uart_should_transmit() {
    return softuart_current._tx_saturated || (softuart_current._txbuf_sent != softuart_current._txbuf_written);
}

bool soft_uart_can_receive() {
    return !softuart_current._rx_saturated;
}

bool soft_uart_has_characters() {
    return softuart_current._rx_saturated || (softuart_current._rxbuf_read != softuart_current._rxbuf_received);
}

bool soft_uart_can_transmit() {
    return !softuart_current._tx_saturated;
}

void soft_uart_write_now(char byte) {
    gpio_write(softuart_current.txpin, LOW);
    delay_us(softuart_current._us_per_bit);

    for (size_t bit = 0; bit < (sizeof(char) * 8); bit++) {
        gpio_write(softuart_current.txpin, (uint8_t) ((byte >> bit) & 1));
        delay_us(softuart_current._us_per_bit);
    }

    gpio_write(softuart_current.txpin, HIGH);
    delay_us(softuart_current._us_per_bit);
}

char soft_uart_read_now() {
    char c = 0;
    delay_us(softuart_current._us_per_bit >> 2);
    for (int bit = 0; bit < (sizeof(char) * 8); bit++) {
        c |= (gpio_read(softuart_current.rxpin) << bit);
        delay_us(softuart_current._us_per_bit);
    }

    return c;
}

void soft_uart_read_interrupt(uint8_t pin) {
    if (!soft_uart_can_receive()) {
        softuart_current._rx_known_dropped++;
        return;
    }

    char c = soft_uart_read_now();

    softuart_current._rxbuf[softuart_current._rxbuf_received] = c;

    softuart_current._rxbuf_received = (softuart_current._rxbuf_received + 1) % SOFT_UART_BUFFER_SIZE;

    if (softuart_current._rxbuf_received == softuart_current._rxbuf_read) {
        softuart_current._rx_saturated = true;
    }

}

void soft_uart_timer_interrupt() {
    while (soft_uart_should_transmit()) {
        char c = softuart_current._txbuf[softuart_current._txbuf_sent];

        softuart_current._txbuf_sent = (softuart_current._txbuf_sent + 1) % SOFT_UART_BUFFER_SIZE;

        soft_uart_write_now(c);

        softuart_current._tx_saturated = false;
    }
}

char soft_uart_getc() {
    while (!soft_uart_has_characters());

    char c = softuart_current._rxbuf[softuart_current._rxbuf_read];

    softuart_current._rxbuf_read = (softuart_current._rxbuf_read + 1) % SOFT_UART_BUFFER_SIZE;

    softuart_current._rx_saturated = false;

    return c;
}

void soft_uart_putc(char c) {
    if (!soft_uart_can_transmit()) {
        return;
    }

    softuart_current._txbuf[softuart_current._txbuf_written] = c;

    softuart_current._txbuf_written = (softuart_current._txbuf_written + 1) % SOFT_UART_BUFFER_SIZE;

    if (softuart_current._txbuf_written == softuart_current._txbuf_sent) {
        softuart_current._tx_saturated = true;
    }
}

void soft_uart_puts(const char* s) {
    for(const char* c = s; *c; c++) {
        soft_uart_putc(*c);
    }
}

void soft_uart_putsn(char* s, size_t size) {
    for(size_t i = 0; i < size; i++) {
        soft_uart_putc(s[i]);
    }
}

void soft_uart_init(uint8_t txpin, uint8_t rxpin, uint32_t us_per_bit) {
    softuart_current.txpin = txpin;
    softuart_current.rxpin = rxpin;
    softuart_current._us_per_bit = us_per_bit;
    softuart_current._txbuf_written = 0;
    softuart_current._txbuf_sent = 0;
    softuart_current._rxbuf_received = 0;
    softuart_current._rxbuf_read = 0;
    softuart_current._tx_saturated = false;
    softuart_current._rx_saturated = false;

    gpio_set_output(txpin);
    gpio_set_input(rxpin);

    gpio_write(txpin, HIGH);

    set_gpio_interrupt(rxpin, soft_uart_read_interrupt);
    gpio_set_detect_event(rxpin, GPIO_EVENT_FALLING_EDGE, true);

    set_timer_interrupt(soft_uart_timer_interrupt);

    softuart_current._initialized = 1;
}

void soft_uart_init_timer_helper(uint32_t us_per_bit) {
    timer_interrupt_setup(RPI_ARMTIMER_CTRL_ENABLE, us_per_bit * 10, RPI_ARMTIMER_CTRL_23BIT,
                          RPI_ARMTIMER_CTRL_INT_ENABLE, RPI_ARMTIMER_CTRL_PRESCALE_1);
}