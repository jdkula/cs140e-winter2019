/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: boolean.h
 * --------------
 * Provides a consistent definition of a boolean
 * across the Pi and x86. Replaces stdbool.h.
 */

#ifndef PIOS_BOOLEAN_H
#define PIOS_BOOLEAN_H

#include "integer.h"

typedef int8_t bool;

enum {
    true = 1,
    false = 0
};

#endif //PIOS_BOOLEAN_H
