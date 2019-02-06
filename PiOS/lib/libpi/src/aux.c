/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: aux.c
 * --------------
 * Provides an interface and constants to interface
 * with the auxiliary module of the Raspberry Pi.
 *
 * This interface is currently partially implemented,
 * with only UART functionality defined.
 */

#include <mem-access.h>
#include "rpi.h"
#include "aux.h"

// BCM2835 ARM Peripherals §1.2.3 - Physical addresses are written at
// 0x7Ennnnnn in the manual, but are memmapped to 0x20nnnnnn.
#define PERIPHERAL_BASE 0x20000000
#define AUX_BASE  (PERIPHERAL_BASE + 0x00215000)

// BCM2835AP §2.1 p. 8 - Register map for +0x40 through +0x68
struct aux_mu_peripherals {
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

// BCM2835AP §2.1 p. 8 - Auxiliary enables
static uint32* const AUX_ENABLES = (void*) (AUX_BASE + 0x4);

// BCM2835AP §2.1 p. 8 - maps to all AUX_MU_* registers.
static struct aux_mu_peripherals* const uart = (void*) (AUX_BASE + 0x40);


void aux_uart_register_enable(uint8 enabled) {
    setBit(AUX_ENABLES, 0, enabled); // BCM2835AP §2.1.1 p. 9 - Mini UART enable
}

void aux_uart_clear(uint8 toClear) { // [toClear] should be 0b010, 0b100, or 0b110.
    put32(&uart->iir, toClear); // BCM2835AP §2.2.2 p. 13 - FIFO clear bits (bits 2:1 "on write")
}

void aux_uart_set_data_size(uint8 dataSize) {
    // Errata: 0b11 must be written to set 8-bit mode. Source: https://elinux.org/BCM2835_datasheet_errata#PDF_Generation
    put32(&uart->lcr, dataSize); // BCM2835AP §2.2.2 p. 14 - data size
}

void aux_uart_enable_uart(uint8 mode) {
    uint32 current = get32(&uart->cntl); // Preserve current settings
    current &= ~(0b11U); // Clear the existing bits,
    current |= mode;     // and put in the requested mode.

    put32(&uart->cntl, current);  // BCM2835AP §2.2.2 p. 17 - Bits 0 and 1, Receiver/Transmitter enable.
}

void aux_uart_set_baudrate(uint16 baudRegRate) {
    put32(&uart->baud, baudRegRate);  // BCM2835AP §2.2.2 p. 19 - AUX_MU_BAUD - Allows direct access to the baudrate counter.
}

/** Resets all registers to their defaults as shown in BCM2835AP §2.2.2. */
void aux_uart_reset_registers() {
    put32(&uart->io, 0);
    put32(&uart->ier, 0);

    // BCM2835AP §2.2.2 p. 13 (Errata: Note the register title incorrectly reads IER instead of IIR)
    put32(&uart->iir, 0b11000001); // Reset sequence as shown on the aforementioned page.
    put32(&uart->lcr, 0);
    put32(&uart->mcr, 0);
    put32(&uart->lsr, 0b100000);  // Reset sequence on BCM2835AP §2.2.2 p. 15
    put32(&uart->msr, 0b100000);  // Reset sequence on BCM2835AP §2.2.2 pp. 15-16
    put32(&uart->scratch, 0);
    put32(&uart->cntl, 0);
    put32(&uart->stat, 0b1100001100);  // Reset sequence on BCM2835AP §2.2.2 p. 18-19
    put32(&uart->baud, 0);
}

bool aux_uart_rx_has_data() {
    return getBit(&uart->stat, 0); // BCM2835AP §2.2.2 p. 19 - Symbol available - Bit is set if RX FIFO has at least 1 symbol.
}

bool aux_uart_tx_has_space() {
    return getBit(&uart->stat, 1); // BCM2835AP §2.2.2 p. 18 - Space available - Bit is set if TX FIFO can accept at least 1 symbol.
}

void aux_uart_transmit(uint8 byte) {
    put32(&uart->io, byte); // BCM2835AP §2.2.2 p. 11 - When writing to this register, data is put into the TX FIFO.
}

uint8 aux_uart_receive() {
    return (uint8) get32(&uart->io); // BCM2835AP §2.2.2 p. 11 - When reading from this register, data is retrieved from the RX FIFO.
}
