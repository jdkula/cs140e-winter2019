/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: timer.c
 * --------------
 * Provides an interface for accessing and interacting
 * with the Raspberry Pi's timer. Additionally
 * allows one to delay application execution for
 * some time.
 */

#include <integer.h>
#include "rpi.h"

// Source: https://github.com/dwelch67/raspberrypi/blob/master/blinker05/blinker05.c
static uint32* const SYSTIMERCLO = (void*) 0x20003004;

void delay(uint32 ticks) {
    for(; ticks > 0; ticks--) {
        dummy(0);  // Do nothing for however many ticks.
    }
}

uint32 timer_get_time(void) {
    return *SYSTIMERCLO;
}

void delay_us(uint32 us) {
    uint32 rb = timer_get_time();
    while (1) {
        uint32 ra = timer_get_time();
        if ((ra - rb) >= us) {
            break;
        }
    }
}

void delay_ms(uint32 ms) {
    delay_us(ms * 1000);
}
