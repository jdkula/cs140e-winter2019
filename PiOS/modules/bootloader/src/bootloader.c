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
#include <arm/boot-load.h>

static void die(unsigned err) {
    put_uint(err);
    delay_ms(100);    // let the output queue drain.
    reboot();
}

// load_code:
//	1. figure out if the requested address range is available.
//	2. copy code to that region.
//	3. return address of first executable instruction: note we have
//	a 8-byte header!  (see ../hello-fixed/memmap)


void notmain(void) {
    uart_init();
    BRANCHTO(load_code());
    clean_reboot();
}