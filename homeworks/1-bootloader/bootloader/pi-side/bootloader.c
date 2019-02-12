/*
 * very simple bootloader.  more robust than xmodem.   (that code seems to
 * have bugs in terms of recovery with inopportune timeouts.)
 */

#define __SIMPLE_IMPL__

#include "rpi.h"
#include "gpio.h"
#include "uart.h"
#include "simple-boot.h"
#include "integer.h"
#include "mem-access.h"
#include "timer.h"

static void die(uint32 code) {
    put_uint(code);
    reboot();
}

//  bootloader:
//	1. wait for SOH, size, cksum from unix side.
//	2. echo SOH, checksum(size), cksum back.
// 	3. wait for ACK.
//	4. read the bytes, one at a time, copy them to ARMBASE.
//	5. verify checksum.
//	6. send ACK back.
//	7. wait 500ms
//	8. jump to ARMBASE.
//

void notmain(void) {
    gpio_init();                            // Initialize diagnostic gpio.

    gpio_set_output(GPIO_ACT);              // Enable ACT & PWR
    gpio_set_output(GPIO_PWR);
    gpio_write(GPIO_PWR, LOW);             // PWR on while waiting...

    uart_init();                        // Enable UART

    while (get_uint() != SOH);          // Wait until SOH. Discard other input.

    gpio_write(GPIO_PWR, LOW);              // Swap PWR and ACT to indicate we're taking in data.
    gpio_write(GPIO_ACT, HIGH);

    uint32 numBytes = get_uint();       // Take in the number of bytes.
    uint32 msgCrc = get_uint();         // Take in the message checksum.

    put_uint(SOH);                      // Let the client know we've got the message

    uint32 nCrc = crc32(&numBytes, 4);  // CRC the number of bytes...
    put_uint(nCrc);                     // ...and send it back to verify.
    put_uint(msgCrc);                   // ...also send back the CRC we were given.

    if(get_uint() == NAK) {             // If the UNIX side doesn't like what we sent, reboot.
        die(NAK);
    }

    if(numBytes > (0x8000000 - 0x8000)) {
        die(NAK);                       // Don't continue if we would overwrite ourself!
    }

    uint32 bytesRead = 0;               // Get ready to read!
    uint32 lastData = get_uint();

    while (lastData != EOT && bytesRead <= numBytes) {
        put32((void*) (ARMBASE + bytesRead), lastData);
        bytesRead += 4;                 // Read 4 bytes at at time. Files are aligned to
        lastData = get_uint();           // 4 bytes on the client.
    }

    if (bytesRead != numBytes) die(NAK); // If we've read too much, die.

                                        // If we've read in corrupt data, die.
    if (crc32((void*) ARMBASE, bytesRead) != msgCrc) die(NAK);

    // We use NAK instead of other messages here, since Dawson
    // told us to do this in lab.

    gpio_write(GPIO_ACT, LOW);          // Visual that we've read everything.

    put_uint(ACK);                      // Let the client know we're OK!


    // XXX: appears we need these delays or the unix side gets confused.
    // I believe it's b/c the code we call re-initializes the uart; could
    // disable that to make it a bit more clean.
    delay_ms(500);

    gpio_write(GPIO_PWR, HIGH);         // Reset GPIO before branch
    gpio_write(GPIO_ACT, LOW);
    gpio_set_input(GPIO_PWR);
    gpio_set_input(GPIO_ACT);

    // run what client sent.
    BRANCHTO(ARMBASE);
    // should not get back here, but just in case.
    reboot();
}
