//
// Created by jonak on 3/10/19.
//

#ifndef PIOS_SOFTUART_H
#define PIOS_SOFTUART_H

#include <integer.h>
#include <boolean.h>

// values in us per bit.
#define SOFT_UART_1200 833U
#define SOFT_UART_2400 416U
#define SOFT_UART_4800 208U
#define SOFT_UART_9600 104U
#define SOFT_UART_19200 52U
#define SOFT_UART_38400 26U
#define SOFT_UART_57600 17U
#define SOFT_UART_115200 8U

#define SOFT_UART_BUFFER_SIZE 4096

struct softuart {
    uint8_t _initialized;
    uint32_t _us_per_bit;
    size_t _txbuf_written;
    size_t _txbuf_sent;
    size_t _rxbuf_received;
    size_t _rxbuf_read;

    bool _tx_saturated;
    bool _rx_saturated;
    uint32_t _rx_known_dropped;

    char _txbuf[SOFT_UART_BUFFER_SIZE];
    char _rxbuf[SOFT_UART_BUFFER_SIZE];

    uint8_t txpin;
    uint8_t rxpin;
};

extern struct softuart softuart_current;

void soft_uart_init(uint8_t txpin, uint8_t rxpin, uint32_t us_per_bit);
void soft_uart_init_timer_helper(uint32_t us_per_bit);

char soft_uart_getc();

void soft_uart_putc(char c);
void soft_uart_puts(const char* s);
void soft_uart_putsn(char* s, size_t size);

bool soft_uart_should_transmit();
bool soft_uart_can_receive();
bool soft_uart_has_characters();
bool soft_uart_can_transmit();

void soft_uart_write_now(char byte);

#endif //PIOS_SOFTUART_H
