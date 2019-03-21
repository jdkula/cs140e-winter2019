/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: softuart.c
 * --------------
 * Defines and implements a complete bit-banging
 * software UART. To maximally emulate a hardware
 * UART and make up a bit for how CPU-intensive it is,
 * all transmitting and receiving is done using
 * timer/GPIO interrupts.
 */

#include <gpio.h>
#include <timer.h>
#include <armtimer.h>
#include <interrupts.h>
#include <printf.h>
#include "softuart.h"


struct softuart softuart_current = {0};

/** Predicate functions that give the state of the UART */

bool soft_uart_should_transmit() {
    return softuart_current._tx_saturated || (softuart_current._txbuf_sent != softuart_current._txbuf_written);
}

bool soft_uart_can_transmit() {
    return !softuart_current._tx_saturated;
}

bool soft_uart_has_characters() {
    return softuart_current._rx_saturated || (softuart_current._rxbuf_read != softuart_current._rxbuf_received);
}

bool soft_uart_can_receive() {
    return !softuart_current._rx_saturated;
}

/** These functions do the actual bit-banging */

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
    // we wait a fourth of the first delay because this caused it to work for me.
    // I think this has to do with the timing of the interrupts (my interrupt vectors are slow)
    delay_us(softuart_current._us_per_bit >> 2);
    for (int bit = 0; bit < (sizeof(char) * 8); bit++) {
        c |= (gpio_read(softuart_current.rxpin) << bit);
        delay_us(softuart_current._us_per_bit);
    }

    return c;
}

/** This function runs when the rx pin detects a falling edge. */
void soft_uart_read_interrupt(uint8_t pin) {
    if (!soft_uart_can_receive()) { // if we can't receive, note that we dropped the packet and finish.
        softuart_current._rx_known_dropped++;
        return;
    }

    char c = soft_uart_read_now();

    // store the received character in the receive buffer, and increment/wraparound the buffer.
    softuart_current._rxbuf[softuart_current._rxbuf_received] = c;
    softuart_current._rxbuf_received = (softuart_current._rxbuf_received + 1) % SOFT_UART_BUFFER_SIZE;

    // if we've caught up with where we're reading from, we've saturated!
    if (softuart_current._rxbuf_received == softuart_current._rxbuf_read) {
        softuart_current._rx_saturated = true;
    }

}

/** This function runs during timer interrupts. */
void soft_uart_timer_interrupt() {
    while (soft_uart_should_transmit()) { // try to flush the transmit buffer.
        char c = softuart_current._txbuf[softuart_current._txbuf_sent];

        softuart_current._txbuf_sent = (softuart_current._txbuf_sent + 1) % SOFT_UART_BUFFER_SIZE;

        soft_uart_write_now(c);

        softuart_current._tx_saturated = false;  // If we've sent, we're no longer saturated.
    }
}

/** Gets a character from the software UART RX buffer, blocking until we have characters to take in. */
char soft_uart_getc() {
    while (!soft_uart_has_characters());

    char c = softuart_current._rxbuf[softuart_current._rxbuf_read];

    softuart_current._rxbuf_read = (softuart_current._rxbuf_read + 1) % SOFT_UART_BUFFER_SIZE;

    softuart_current._rx_saturated = false;

    return c;
}

/** Put a character in the software UART TX buffer, silently failing if the buffer is full. */
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

/** Places the entire null-terminated string into the TX buffer. */
void soft_uart_puts(const char* s) {
    for(const char* c = s; *c; c++) {
        soft_uart_putc(*c);
    }
}

/** Places [size] bytes into the TX buffer. This function passes over null-terminators. */
void soft_uart_putsn(char* s, size_t size) {
    for(size_t i = 0; i < size; i++) {
        soft_uart_putc(s[i]);
    }
}

/** Initializes GPIO pins and the singleton softuart struct. */
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

/** Sets up timer interrupts given the "baud rate.". */
void soft_uart_init_timer_helper(uint32_t us_per_bit) {
    timer_interrupt_setup(RPI_ARMTIMER_CTRL_ENABLE, us_per_bit * 10, RPI_ARMTIMER_CTRL_23BIT,
                          RPI_ARMTIMER_CTRL_INT_ENABLE, RPI_ARMTIMER_CTRL_PRESCALE_1);
}