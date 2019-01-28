/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: aux.h
 * --------------
 * Provides an interface and constants to interface
 * with the auxiliary module of the Raspberry Pi.
 *
 * This interface is currently partially implemented,
 * with only UART functionality defined.
 */

#ifndef PIOS_AUX_UART_H
#define PIOS_AUX_UART_H

#include <integer.h>
#include <boolean.h>

/** Setting to enable the UART and UART registers. */
void aux_uart_enable_set(uint8 enabled);

/** Constants for aux_uart_enable_set. */
enum {
    UART_ENABLE = 1,
    UART_DISABLE = 0
};

/** Clears the UART's TX and/or RX buffers, depending on the constant passed in. */
void aux_uart_clear(uint8 toClear);

/** Constants for aux_uart_clear. You can OR these to clear both. */
enum {
    UART_TX_CLEAR = 0b010,
    UART_RX_CLEAR = 0b100
};

/** Sets the data size used by the UART. */
void aux_uart_set_data_size(uint8 dataSize);

/** Constants used by aux_uart_set_data_size */
enum {
    UART_SEVEN_BIT_MODE = 0,   // BCM2835AP §2.2.2 p. 14 - data size - write 0 for 7-bit mode.
    UART_EIGHT_BIT_MODE = 0b11 // Errata: 0b11 must be written to set 8-bit mode. Source: https://elinux.org/BCM2835_datasheet_errata#PDF_Generation
};

/** Enables sending and/or recieving using the UART. The absence of a flag disables it. */
void aux_uart_enable_uart(uint8 mode);

/** Constants used by aux_uart_enable_uart. May be OR'd together to enable both. */
enum {
    UART_TX_ENABLE = 0b10, // BCM2835AP §2.2.2 p. 17 - Transmitter enable @ bit 1
    UART_RX_ENABLE = 0b1   // BCM2835AP §2.2.2 p. 17 - Receiver enable @ bit 0
};

/** Sets the baudrate used when communicating. */
void aux_uart_set_baudrate(uint16 baudRegRate);

/** Constants used by aux_uart_set_baudrate. Note that the baud rate and the resulting constant are different! */
enum {
    UART_BAUD_115200 = 270
};

/** Sets all UART registers to their defaults. */
void aux_uart_reset_registers();

/** Returns true if the RX buffer has data to pull. */
bool aux_uart_rx_has_data();

/** Returns true if the TX buffer has space for additional data to send. */
bool aux_uart_tx_has_space();

/** Transmits a [byte] into the UART TX buffer, blocking until the buffer has space. */
void aux_uart_transmit(uint8 byte);

/** Takes in a [byte] from the UART RX buffer, blocking until some data is recieved. */
uint8 aux_uart_receive();

#endif //PIOS_AUX_UART_H
