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

#include "arm/boot-impl.h"


static void die(uint32_t code) {
    put_uint(code);
    delay_ms(200);
    reboot();
}


// load_code:
//	1. figure out if the requested address range is available.
//	2. copy code to that region.
//	3. return address of first executable instruction: note we have
//	a 8-byte header!  (see ../hello-fixed/memmap)
int load_code(void) {
    unsigned addr = 0;

    gpio_init();                            // Initialize diagnostic gpio.

    gpio_set_output(GPIO_ACT);              // Enable ACT & PWR
    gpio_set_output(GPIO_PWR);
    gpio_write(GPIO_PWR, LOW);             // PWR on while waiting...

    uart_init();                        // Enable UART

    while (uart_getc() != SOH);          // Wait until SOH. Discard other input.

    gpio_write(GPIO_PWR, LOW);              // Swap PWR and ACT to indicate we're taking in data.
    gpio_write(GPIO_ACT, HIGH);

    put_uint(ACK);

    uint32_t version = get_uint();
    if(version != 2) {
        die(NAK);
    }
    addr = get_uint();

    uint32_t numBytes = get_uint();       // Take in the number of bytes.
    uint32_t msgCrc = get_uint();         // Take in the message checksum.

    uint32_t nCrc = crc32(&numBytes, 4);  // CRC the number of bytes...
    put_uint(nCrc);                     // ...and send it back to verify.
    put_uint(msgCrc);                   // ...also send back the CRC we were given.

    if (get_uint() == NAK) {             // If the UNIX side doesn't like what we sent, reboot.
        die(NAK);
    }

    put_uint(ACK);

    uint32_t bytesRead = 0;               // Get ready to read!
    uint32_t lastData = get_uint();

    while (lastData != EOT) {
        IGNORE(-Wint - to - pointer - cast);
        put32((void*) (addr + bytesRead), lastData);
        POP();
        bytesRead += 4;
        lastData = get_uint();
        if (uart_errno == UART_ERR_TIMEOUT) {
            gpio_write(GPIO_ACT, LOW);
            gpio_write(GPIO_PWR, HIGH);
            delay(100000);
            reboot();
            return -1;
        }
    }


    if (bytesRead != numBytes) die(SIZE_MISMATCH);

    if (crc32((void*) addr, bytesRead) != msgCrc) die(BAD_CKSUM);

    gpio_write(GPIO_ACT, LOW);

    put_uint(ACK);


    // XXX: appears we need these delays or the unix side gets confused.
    // I believe it's b/c the code we call re-initializes the uart; could
    // disable that to make it a bit more clean.
    delay_ms(500);

    gpio_write(GPIO_PWR, HIGH);
    gpio_write(GPIO_ACT, LOW);
    gpio_set_input(GPIO_PWR);
    gpio_set_input(GPIO_ACT);

    // run what client sent.
    BRANCHTO(addr + 8); // +8 for version + address!

    // will return here when it's done!
    return 0;
}
