#include <uart.h>
#include <timer.h>
#include <mem-access.h>
#include "rpi.h"
#include "vmm/pi-vmm-ops.h"

// note: printf will not work unless you do something special.
#undef printf
#define printf  cannot_print

// probably should put reply's and CRC32
int notmain(void) {
    uart_init();
    delay_ms(100);

    put_uint(OP_READY);
    while (1) {
        /* have loop to handle requests, including OP_REBOOT */
        unsigned op = get_uint();

        if (op == OP_READ32) {
            unsigned address = get_uint();
            put_uint(get32((void*) address));
        } else if (op == OP_WRITE32) {
            unsigned address = get_uint();
            unsigned value = get_uint();
            put32((void*) address, value);
        } else if (op == OP_DONE || op == OP_ERROR || op == OP_REBOOT) {
            reboot();
        }
    }
}
