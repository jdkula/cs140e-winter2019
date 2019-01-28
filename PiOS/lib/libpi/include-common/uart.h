/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: uart.h
 * --------------
 * Provides an interface for the Raspberry Pi's miniUART.
 */

#ifndef UART_DEFS
#define UART_DEFS

#include <integer.h>

/** Sets up the miniUART for use. */
void uart_init(void);

/** Gets a character from the peer, blocking until something is recieved. */
uint8 uart_getc(void);

/** Sends a character to the peer, blockiing until it is queued for sending. */
void uart_putc(uint8 c);

#endif
