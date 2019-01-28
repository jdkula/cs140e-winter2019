/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: reboot.c
 * --------------
 * Provides reboot functionality declared in rpi.h.
 */

#include <mem-access.h>
#include <timer.h>
#include "rpi.h"

// Reboot Method Source: https://github.com/dwelch67/raspberrypi/blob/master/blinker06/wdog.c
void reboot(void) {
    void* const PM_RSTC = (void*) 0x2010001c;
    void* const PM_WDOG = (void*) 0x20100024;
    const int PM_PASSWORD = 0x5a000000;
    const int PM_RSTC_WRCFG_FULL_RESET = 0x00000020;

    delay(100000);

    // timeout = 1/16th of a second? (whatever)
    put32(PM_WDOG, PM_PASSWORD | 1);
    put32(PM_RSTC, PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET);
    while (1);
}
