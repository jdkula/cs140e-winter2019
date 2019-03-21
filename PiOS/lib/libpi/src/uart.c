/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: uart.h
 * --------------
 * Provides an interface for the Raspberry Pi's miniUART.
 */

#include <printf.h>
#include "rpi.h"
#include "uart.h"
#include "gpio.h"
#include "mem-barrier.h"
#include "aux.h"

/** This global keeps track of UART errors. */
int8_t uart_errno = UART_ERR_OK;

void uart_init(void) {
    if(aux_uart_get_enabled() != 0b11) {
        uart_force_init();
    }
}

void uart_force_init(void) {
    data_sync_barrier();

    // Start by setting up GPIO.
    gpio_set_function(GPIO_TX, GPIO_FUNC_UART); // BCM2835AP §6.2 p. 102
    gpio_set_function(GPIO_RX, GPIO_FUNC_UART);
    data_sync_barrier();

    // Then, enable the UART registers.
    aux_uart_register_enable(UART_ENABLE);
    data_sync_barrier();

    // Now that the registers are enabled, let's reset them to complete defaults.
    aux_uart_reset_registers();
    data_sync_barrier();

    // Next, our settings. We're using a baud rate of 115200 in 8-bit mode.
    aux_uart_set_baudrate(UART_BAUD_9600);
    aux_uart_set_data_size(UART_EIGHT_BIT_MODE);

    // Finally, before we enable TX/RX, let's clear their buffers.
    aux_uart_clear(UART_TX_CLEAR | UART_RX_CLEAR);
    data_sync_barrier();

    // Before returning, enable TX and RX.
    aux_uart_enable_uart(UART_TX_ENABLE | UART_RX_ENABLE);
    data_sync_barrier();
}

uint8_t uart_getc() {
    // Do nothing until the RX buffer has data, or until we time out. If the timeout is 0, wait forever.
//    for (; (timeout_ticks > 1 || timeout_ticks == 0) && !aux_uart_rx_has_data(); timeout_ticks--);
//    if (timeout_ticks == 1) {
//        uart_errno = UART_ERR_TIMEOUT;
//        return UART_ERROR;
//    }

    while (!aux_uart_rx_has_data());

    uart_errno = UART_ERR_OK;
    return aux_uart_receive();
}

uint8_t uart_putc(uint8_t c) {
    // Do nothing until the TX buffer has space, or until we time out. If the timeout is 0, wait forever.
//    for (; (timeout_ticks > 1 || timeout_ticks == 0) && !aux_uart_tx_has_space(); timeout_ticks--);
//    if (timeout_ticks == 1) {
//        uart_errno = UART_ERR_TIMEOUT;
//        return UART_ERROR;
//    }
    while (!aux_uart_tx_has_space());


    aux_uart_transmit(c);

    uart_errno = UART_ERR_OK;
    return 0;
}

uint32_t get_uint(void) {
    uint32_t u = uart_getc();
    u |= uart_getc() << 8u;
    u |= uart_getc() << 16u;
    u |= uart_getc() << 24u;
    return u;
}

void put_uint(uint32_t u) {
    uart_putc((u >> 0u) & 0xffu);
    uart_putc((u >> 8u) & 0xffu);
    uart_putc((u >> 16u) & 0xffu);
    uart_putc((u >> 24u) & 0xffu);
}

// Works with our UART receivers, which quit upon seeing the string "DONE!!!"
void clean_reboot(void) {
    printk("DONE!!!\n");
    data_sync_barrier();
    reboot();
}