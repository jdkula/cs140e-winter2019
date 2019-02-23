/*
 * very simple bootloader.  more robust than xmodem.   (that code seems to
 * have bugs in terms of recovery with inopportune timeouts.)
 */

#define __SIMPLE_IMPL__

#include <timer.h>
#include <rpi.h>
#include <gpio.h>
#include <integer.h>
#include <crc32.h>
#include <uart.h>
#include <mem-access.h>
#include <debug.h>
#include <boot-messages.h>
#include <printf.h>
#include <pios-macros.h>

static void die(unsigned err) {
    put_uint(err);
    delay_ms(100); 	// let the output queue drain.
    reboot();
}

// load_code:
//	1. figure out if the requested address range is available.
//	2. copy code to that region.
//	3. return address of first executable instruction: note we have
//	a 8-byte header!  (see ../hello-fixed/memmap)
int load_code(void) {
    gpio_init();
    gpio_set_output(GPIO_ACT);
    gpio_set_output(GPIO_PWR);

    unsigned addr=0;

    while(uart_getc() != SOH);

    // let unix know we are ready.
    gpio_write(GPIO_ACT, HIGH);
    put_uint(ACK);

    uint32_t version = get_uint();
    gpio_write(GPIO_ACT, LOW);
    if(version != 2) {
        die(NAK);
    }
    addr = get_uint();

    uint32_t numBytes = get_uint();       // Take in the number of bytes.
    uint32_t msgCrc = get_uint();         // Take in the message checksum.

    uint32_t nCrc = crc32(&numBytes, 4);  // CRC the number of bytes...
    put_uint(nCrc);                     // ...and send it back to verify.
    put_uint(msgCrc);                   // ...also send back the CRC we were given.

    if (get_uint() != ACK) {             // If the UNIX side doesn't like what we sent, reboot.
        die(NAK);
    }

    put_uint(ACK);

    for(int i = 0; i < numBytes; i += 4) {
        unsigned data = get_uint();
        put32(addr + i, data);
    }

    if (get_uint() != EOT) die(SIZE_MISMATCH);

    if (crc32((void*) addr, numBytes) != msgCrc) die(BAD_CKSUM);


    put_uint(ACK);

    // give time to flush out; ugly.   implement `uart_flush()`
    delay_ms(100);

    BRANCHTO(addr + 8); // +8 for version + address!

    /* return address */
}

void notmain(void) {
    uart_init();
    load_code();
    clean_reboot();
}