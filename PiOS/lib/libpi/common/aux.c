//
// Created by jonak on 1/23/19.
//

#include <mem-access.h>
#include "rpi.h"
#include "aux.h"

struct aux_periphs {
    uint32 io;
    uint32 ier;
    uint32 iir;
    uint32 lcr;
    uint32 mcr;
    uint32 lsr;
    uint32 msr;
    uint32 scratch;
    uint32 cntl;
    uint32 stat;
    uint32 baud;
};

static uint32* const AUX_ENABLES = (void*) (AUX_BASE + 0x4);

static struct aux_periphs* const uart = (void*) (AUX_BASE + 0x40);


void aux_uart_enable_set(uint8 enabled) {
    setBit(AUX_ENABLES, 0, enabled); // Bottom of page 9
}

void aux_uart_clear(uint8 toClear) { // Middle of Page 13
    put32(&uart->iir, toClear);
}

void aux_uart_set_data_size(uint8 dataSize) {
    put32(&uart->lcr, dataSize);
}

void aux_uart_enable_uart(uint8 mode) {
    uint32 current = get32(&uart->cntl);
    current &= ~(0b11U);
    current |= mode;

    put32(&uart->cntl, current);
}

void aux_uart_set_baudrate(uint16 baudRegRate) {
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

uint8 aux_uart_rx_has_data() {
    return getBit(&uart->stat, 0); // Page 19
}

uint8 aux_uart_tx_has_space() {
    return getBit(&uart->stat, 1); // Page 18 @ bottom
}

void aux_uart_transmit(uint8 byte) {
    put32(&uart->io, byte);
}

uint8 aux_uart_receive() {
    return (uint8) get32(&uart->io);
}
