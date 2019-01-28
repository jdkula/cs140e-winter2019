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

static void send_byte(uint8 uc) {
    uart_putc(uc);
}

static uint8 get_byte(void) {
    return uart_getc();
}

static uint32 get_uint(void) {
    uint32 u = get_byte();
    u |= get_byte() << 8u;
    u |= get_byte() << 16u;
    u |= get_byte() << 24u;
    return u;
}

static void put_uint(uint32 u) {
    send_byte((u >> 0u) & 0xffu);
    send_byte((u >> 8u) & 0xffu);
    send_byte((u >> 16u) & 0xffu);
    send_byte((u >> 24u) & 0xffu);
}

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

void module_main(void) {
    gpio_init();

    debug_enable(GPIO_ACT);
    debug_enable(GPIO_PWR);
    debug_enable(GPIO_PIN26);
    debug_enable(GPIO_PIN20);

    uart_init();

    uint32 lastMessage = get_uint();
    while (lastMessage != SOH);

    uint32 numBytes = get_uint();
    uint32 msgCrc = get_uint();

    put_uint(SOH);

    uint32 bytesCrc = crc32(&numBytes, 4);
    put_uint(bytesCrc);

    put_uint(msgCrc);

    uint32 bytesRead = 0;
    uint32 lastData = get_uint();
    //bytesRead += 4;

    while (lastData != EOT) {
        put_uint(lastData);
        put32((void*) (ARMBASE + bytesRead), lastData);
        bytesRead += 4;
        lastData = get_uint();
        if (lastData == 0x40404040) {
            debug_on(GPIO_ACT);
        }
//        if(uart_errno == UART_ERR_TIMEOUT) {
//            reboot();
//            return;
//        }
    }


    if (bytesRead != numBytes) die(SIZE_MISMATCH);

    if (crc32((void*) ARMBASE, bytesRead) != msgCrc) die(NAK);

    put_uint(ACK);


    // XXX: appears we need these delays or the unix side gets confused.
    // I believe it's b/c the code we call re-initializes the uart; could
    // disable that to make it a bit more clean.
    delay_ms(500);

    // run what client sent.
    BRANCHTO(ARMBASE);
    // should not get back here, but just in case.
    reboot();
}