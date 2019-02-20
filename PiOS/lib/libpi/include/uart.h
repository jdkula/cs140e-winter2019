/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: uart.h
 * --------------
 * Provides an interface for the Raspberry Pi's miniUART.
 */

#ifndef UART_DEFS
#define UART_DEFS

#include <integer.h>

/** Functions return UART_ERROR and set uart_errno when there's an issue. */
#define UART_ERROR 127;
enum {
    UART_ERR_OK,       // Indicates the absence of an error.
    UART_ERR_TIMEOUT   // Indicates that a timeout occurred while waiting to sent/receive data.
};
extern int8_t uart_errno;

/** Sets up the miniUART for use. */
void uart_init(void);

/**
 * Gets a character from the peer, blocking until something is recieved or the timeout is reached.
 * Pass a timeout of 0 to wait forever. This function will wait for [timeout_ticks]-1 ticks.
 */
uint8_t uart_getc();

/**
 * Sends a character to the peer, blocking until something is recieved or the timeout is reached.
 * Pass a timeout of 0 to wait forever. This function will wait for [timeout_ticks]-1 ticks.
 */
uint8_t uart_putc(uint8_t c);

/** Gets an integer from the UART. */
unsigned get_uint(void);

/** Sends an integer to the peer. */
void put_uint(unsigned u);

/** Sends a sentinel over the UART to indicate the Pi is shutting down, then reboots. */
void clean_reboot(void);
#endif
