/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: integer.h [ARM Implementation]
 * --------------
 * Provides a consistent definition of ints
 * of various signededness and widths across platforms.
 */

#ifndef PIOS_INTEGER_H
#define PIOS_INTEGER_H

// We assume a 32-bit uint32 because of the Raspberry Pi.

#ifdef __arm__
typedef long long int64_t;
typedef unsigned long long uint64_t;

typedef int int32_t;
typedef unsigned uint32_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef char int8_t;
typedef unsigned char uint8_t;

typedef unsigned size_t;

#else

#include <stddef.h>
#include <stdint.h>

#endif

#endif //PIOS_INTEGER_H
