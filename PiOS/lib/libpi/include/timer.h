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
void delay(uint32_t ticks);

/** Gets the current time as reported by the Pi's timer. */
uint32_t timer_get_time(void);

/** Delays program execution for a given number of [us]. */
void delay_us(uint32_t us);

/** Delays program execution for a given number of [ms]. */
void delay_ms(uint32_t ms);

void timer_interrupt_setup(uint32_t enabled, uint32_t ncycles, uint32_t bits, uint32_t interrupt_enabled, uint32_t prescale);
