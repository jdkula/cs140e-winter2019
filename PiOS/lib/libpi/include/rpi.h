/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: rpi.h
 * --------------
 * Provides access to other operations that relate
 * to the Raspberry Pi as a system.
 */

#ifndef __RPI_H__
#define __RPI_H__

#include <integer.h>

/** Reboots the Raspberry Pi. */
void reboot(void);

/** Does nothing -- just uses a CPU cycle. */
void dummy(uint32_t unused);

#endif // __RPI_H__