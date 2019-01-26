//
// Created by jonak on 1/23/19.
//

#ifndef LAB4_UART_AUX_H
#define LAB4_UART_AUX_H

#include "integer.h"

#define PERIPHERAL_BASE 0x20200000
#define AUX_BASE  (PERIPHERAL_BASE + 0x00015000)

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

void aux_uart_enable_set(uint8 enabled);
void aux_uart_clear(uint8 toClear);
void aux_uart_set_data_size(uint8 dataSize);
void aux_uart_enable_uart(uint8 mode);
void aux_uart_set_baudrate(uint16 baudRegRate);
void aux_uart_reset_registers();

uint8 aux_uart_rx_has_data();
uint8 aux_uart_tx_has_space();

void aux_uart_transmit(uint8 byte);
uint8 aux_uart_receive();

#endif //LAB4_UART_AUX_H
