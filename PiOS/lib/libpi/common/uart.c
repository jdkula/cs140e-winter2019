#include "rpi.h"
#include "uart.h"
#include "gpio.h"
#include "mem-barrier.h"
#include "aux.h"

#define PIN_TX 14
#define PIN_RX 15

// use this if you need memory barriers.
void dev_barrier(void) {
    dmb();
    dsb();
}

void uart_init(void) {
    dev_barrier();
    gpio_set_function(PIN_TX, GPIO_FUNC_ALT5); // 6.2
    gpio_set_function(PIN_RX, GPIO_FUNC_ALT5);
    dev_barrier();

    aux_uart_enable_set(ENABLED);
    dev_barrier();

    aux_uart_reset_registers();
    dev_barrier();

    aux_uart_set_baudrate(BAUD_115200);
    aux_uart_set_data_size(EIGHT_BIT_MODE);
    aux_uart_clear(TX_CLEAR | RX_CLEAR);
    dev_barrier();

    aux_uart_enable_uart(TX_ENABLE | RX_ENABLE);
    dev_barrier();
}

int uart_getc(void) {
    while (!aux_uart_rx_has_data());
    return aux_uart_receive();
}

void uart_putc(unsigned c) {
    while (!aux_uart_tx_has_space());
    aux_uart_transmit((unsigned char) c);
}
