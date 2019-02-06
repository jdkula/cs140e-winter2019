//
// Created by jonak on 2/6/19.
//

#ifndef PIOS_UART_X86_EXTENSIONS_H
#define PIOS_UART_X86_EXTENSIONS_H


/** Stores the Raspberry Pi's file descriptor. */
extern int rpi_fd;

/** Initializes the communication file descriptor for the RPi */
void aux_init(int fd);

#endif //PIOS_UART_X86_EXTENSIONS_H
