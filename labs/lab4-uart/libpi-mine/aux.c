//
// Created by jonak on 1/23/19.
//

#include <rpi.h>
#include "aux.h"
#include "helper.h"

unsigned *AUX_ENABLES = (void *) (AUX_BASE + 0x4);

static struct aux_periphs *const uart = (void *) (AUX_BASE + 0x40);


void aux_uart_enable_set(unsigned char enabled) {
    setBit(AUX_ENABLES, 0, enabled); // Bottom of page 9
}

void aux_uart_clear(unsigned char toClear) { // Middle of Page 13
    put32(&uart->iir, toClear);
}

void aux_uart_set_data_size(unsigned char dataSize) {
    put32(&uart->lcr, dataSize);
}

void aux_uart_enable_uart(unsigned char mode) {
    unsigned int current = get32(&uart->cntl);
    current &= ~(0b11);
    current |= mode;

    put32(&uart->cntl, current);
}

void aux_uart_set_baudrate(unsigned short baudRegRate) {
    put32(&uart->baud, baudRegRate);
}

void aux_uart_reset_registers() {
    put32(&uart->io, 0);
    put32(&uart->ier, 0);
    put32(&uart->iir, 0b11000001);
    put32(&uart->lcr, 0);
    put32(&uart->mcr, 0);
    put32(&uart->lsr, 0b100000);
    put32(&uart->msr, 0b100000);
    put32(&uart->scratch, 0);
    put32(&uart->cntl, 0);
    put32(&uart->stat, 0b1100001100);
    put32(&uart->baud, 0);
}

unsigned char aux_uart_rx_has_data() {
    return getBit(&uart->stat, 0); // Page 19
}

unsigned char aux_uart_tx_has_space() {
    return getBit(&uart->stat, 1); // Page 18 @ bottom
}

void aux_uart_transmit(unsigned char byte) {
    put32(&uart->io, byte);
}

unsigned char aux_uart_receive() {
    return (unsigned char) get32(&uart->io);
}
