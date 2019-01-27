/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: debug.c [ARM Implementation]
 * --------------
 * Provides an interface to deal with GPIO for
 * debugging purposes (e.g. turning on LEDs to
 * indicate progress, etc.)
 *
 * These can all be disabled by compiling with the
 * macro "DEBUG_OFF" defined.
 */

#include <gpio.h>
#include <timer.h>
#include <debug.h>
#include "debug.h"

void debug_on(uint8 pin) {
#ifndef DEBUG_OFF
    gpio_write(pin, 1);
#endif
}

void debug_off(uint8 pin) {
#ifndef DEBUG_OFF
    gpio_write(pin, 0);
#endif
}

void debug_enable(uint8 pin) {
#ifndef DEBUG_OFF
    gpio_set_function(pin, GPIO_FUNC_OUTPUT);
#endif
}

void debug_disable(uint8 pin) {
#ifndef DEBUG_OFF
    // Input was chosen as the "disable" state since it's the default pin function.
    // See BCM2835-ARM-Peripherals p. 91 -- "All pins reset to normal GPIO input operation."
    gpio_set_function(pin, GPIO_FUNC_INPUT);
#endif
}