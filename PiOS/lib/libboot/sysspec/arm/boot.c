//
// Created by jonak on 2/20/19.
//

#include <uart.h>
#include <boot-messages.h>
#include <demand.h>
#include <timer.h>
#include <rpi.h>
#include <gpio.h>
#include <crc32.h>
#include <pios-macros.h>
#include <mem-access.h>

#include "arm/boot-load.h"


static void die(uint32_t code) {
    put_uint(code);
    gpio_write(GPIO_ACT, HIGH);
    gpio_write(GPIO_PWR, HIGH);
    delay_ms(100);
    gpio_write(GPIO_ACT, LOW);
    delay_ms(50);
    gpio_write(GPIO_PWR, LOW);
    gpio_write(GPIO_ACT, HIGH);
    delay_ms(50);
    reboot();
}


// load_code:
//	1. figure out if the requested address range is available.
//	2. copy code to that region.
//	3. return address of first executable instruction: note we have
//	a 8-byte header!  (see ../hello-fixed/memmap)
uint32_t load_code(void) {
    gpio_init();
    gpio_set_output(GPIO_ACT);
    gpio_set_output(GPIO_PWR);

    unsigned addr = 0;

    uart_init();

    while (uart_getc() != SOH);

    // let unix know we are ready.
    gpio_write(GPIO_ACT, HIGH);
    put_uint(ACK);

    uint32_t version = get_uint();
    addr = get_uint();

    uint32_t numBytes = get_uint();       // Take in the number of bytes.
    uint32_t msgCrc = get_uint();         // Take in the message checksum.

    uint32_t nCrc = crc32(&numBytes, 4);  // CRC the number of bytes...
    put_uint(nCrc);                     // ...and send it back to verify.
    put_uint(msgCrc);                   // ...also send back the CRC we were given.

    if (get_uint() != ACK) {             // If the UNIX side doesn't like what we sent, reboot.
        die(NAK);
    }

    if (version != 2) {
        die(BAD_VERSION);
    }

    if (addr < LAST_USED_ADDRESSES || addr > MAX_ADDRESS) {
        die(BAD_START);
    }

    if ((addr + numBytes) > MAX_ADDRESS) {
        die(BAD_END);
    }

    put_uint(ACK);

    for (int i = 0; i < numBytes; i += 4) {
        unsigned data = get_uint();
        put32(addr + i, data);
    }

    if (get_uint() != EOT) die(SIZE_MISMATCH);

    if (crc32((void*) addr, numBytes) != msgCrc) die(BAD_CKSUM);


    put_uint(ACK);

    // give time to flush out; ugly.   implement `uart_flush()`
    delay_ms(100);

    gpio_set_input(GPIO_ACT);
    gpio_set_input(GPIO_PWR);

    return (addr + 8);
}