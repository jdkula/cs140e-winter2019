/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: debug.c [X86 Implementation]
 * --------------
 * Provides an interface to deal with GPIO for
 * debugging purposes (e.g. turning on LEDs to
 * indicate progress, etc.)
 *
 * This x86 implementation just writes to the console
 * instead of using the faux/test-harness GPIO code,
 * since the intention of this code is to make it easier
 * where bugs exist and where the Pi is getting tripped up.
 *
 * These can all be disabled by compiling with the
 * macro "DEBUG_OFF" defined.
 */

#include <stdio.h>
#include <integer.h>
#include "debug.h"

void debug_on(uint8 pin) {
#ifndef DEBUG_OFF
    printf("[Debug] pin %d turned on\n", pin);
#endif
}

void debug_off(uint8 pin) {
#ifndef DEBUG_OFF
    printf("[Debug] pin %d turned off\n", pin);
#endif
}

void debug_enable(uint8 pin) {
#ifndef DEBUG_OFF
    printf("[Debug] pin %d enabled for debugging\n", pin);
#endif
}

void debug_disable(uint8 pin) {
#ifndef DEBUG_OFF
    printf("[Debug] pin %d disabled for debugging\n", pin);
#endif
}

void debug_delay(uint32 ticks) {
#ifndef DEBUG_OFF
    printf("[Debug] Delayed for %u ticks\n", ticks);
#endif
}

