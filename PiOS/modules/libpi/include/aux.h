//
// Created by jonak on 1/23/19.
//

#ifndef LAB4_UART_AUX_H
#define LAB4_UART_AUX_H

#define PERIPHERAL_BASE 0x20200000
#define AUX_BASE  (PERIPHERAL_BASE + 0x00015000)

struct aux_periphs {
    unsigned io;
    unsigned ier;
    unsigned iir;
    unsigned lcr;
    unsigned mcr;
    unsigned lsr;
    unsigned msr;
    unsigned scratch;
    unsigned cntl;
    unsigned stat;
    unsigned baud;
};

enum {
    ENABLED = 1,
    DISABLED = 0
};

enum {
    TX_CLEAR = 0b010,
    RX_CLEAR = 0b100
};

enum {
    SEVEN_BIT_MODE = 0,
    EIGHT_BIT_MODE = 0b11
};

enum {
    TX_ENABLE = 0b10,
    RX_ENABLE = 0b1
};

enum {
    BAUD_115200 = 270
};

void aux_uart_enable_set(unsigned char enabled);
void aux_uart_clear(unsigned char toClear);
void aux_uart_set_data_size(unsigned char dataSize);
void aux_uart_enable_uart(unsigned char mode);
void aux_uart_set_baudrate(unsigned short baudRegRate);
void aux_uart_reset_registers();

unsigned char aux_uart_rx_has_data();
unsigned char aux_uart_tx_has_space();

void aux_uart_transmit(unsigned char byte);
unsigned char aux_uart_receive();

#endif //LAB4_UART_AUX_H
