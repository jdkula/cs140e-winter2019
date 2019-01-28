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
extern int8 uart_errno;

/** Sets up the miniUART for use. */
void uart_init(void);

/**
 * Gets a character from the peer, blocking until something is recieved or the timeout is reached.
 * Pass a timeout of 0 to wait forever. This function will wait for [timeout_ticks]-1 ticks.
 */
uint8 uart_getc();

/**
 * Sends a character to the peer, blocking until something is recieved or the timeout is reached.
 * Pass a timeout of 0 to wait forever. This function will wait for [timeout_ticks]-1 ticks.
 */
void uart_putc(uint8 c);

#endif
