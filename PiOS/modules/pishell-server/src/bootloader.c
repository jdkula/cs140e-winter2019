/* simplified bootloader.  */
#include <uart.h>
#include <timer.h>
#include <demand.h>
#include <rpi.h>
#include <boot-messages.h>
#include "pi-shell.h"

#define __SIMPLE_IMPL__

static void send_byte(unsigned char uc) {
    uart_putc(uc);
}

static unsigned char get_byte(void) {
    return uart_getc();
}

static void die(unsigned err) {
    put_uint(err);
    delay_ms(100);    // let the output queue drain.
    reboot();
}

