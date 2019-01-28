#include "rpi.h"
#include "uart.h"
#include "gpio.h"
#include "mem-barrier.h"
#include "aux.h"

// use this if you need memory barriers.
void dev_barrier(void) {
    data_memory_barrier();
    data_sync_barrier();
}

void uart_init(void) {
    dev_barrier();
    gpio_set_function(GPIO_TX, GPIO_FUNC_ALT5); // 6.2
    gpio_set_function(GPIO_RX, GPIO_FUNC_ALT5);
    dev_barrier();

    aux_uart_enable_set(UART_ENABLE);
    dev_barrier();

    aux_uart_reset_registers();
    dev_barrier();

    aux_uart_set_baudrate(UART_BAUD_115200);
    aux_uart_set_data_size(UART_EIGHT_BIT_MODE);
    aux_uart_clear(UART_TX_CLEAR | UART_RX_CLEAR);
    dev_barrier();

    aux_uart_enable_uart(UART_TX_ENABLE | UART_RX_ENABLE);
    dev_barrier();
}

uint8 uart_getc(void) {
    while (!aux_uart_rx_has_data());
    return aux_uart_receive();
}

void uart_putc(uint8 c) {
    while (!aux_uart_tx_has_space());
    aux_uart_transmit(c);
}
