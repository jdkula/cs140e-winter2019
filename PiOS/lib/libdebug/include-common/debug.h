/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: debug.h
 * --------------
 * Provides an interface to deal with GPIO for
 * debugging purposes (e.g. turning on LEDs to
 * indicate progress, etc.)
 *
 * These can all be disabled by compiling with the
 * macro "DEBUG_OFF" defined.
 */

#ifndef PIOS_DEBUG_H
#define PIOS_DEBUG_H

/** Sets the given [pin] to HIGH */
void debug_on(uint8 pin);

/** Sets the given [pin] to LOW */
void debug_off(uint8 pin);

/** Sets the given [pin]'s function to OUTPUT. */
void debug_enable(uint8 pin);

/** Sets the given [pin]'s function to INPUT. */
void debug_disable(uint8 pin);

/** Delays for the given number of [ticks]. */
void debug_delay(uint32 ticks);

#endif //PIOS_DEBUG_H
