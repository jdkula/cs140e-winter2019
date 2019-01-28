/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: timer.h
 * --------------
 * Provides an interface for accessing and interacting
 * with the Raspberry Pi's timer. Additionally
 * allows one to delay application execution for
 * some time.
 */

#include <integer.h>

/** Delays for [ticks] CPU instructions. */
void delay(uint32 ticks);

/** Gets the current time as reported by the Pi's timer. */
uint32 timer_get_time(void);

/** Delays program execution for a given number of [us]. */
void delay_us(uint32 us);

/** Delays program execution for a given number of [ms]. */
void delay_ms(uint32 ms);

