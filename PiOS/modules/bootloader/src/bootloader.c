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
#include <arm/boot-impl.h>


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
    load_code();
    clean_reboot();
}