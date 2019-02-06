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
typedef long long int64;
typedef unsigned long long uint64;

typedef int int32;
typedef unsigned uint32;

typedef short int16;
typedef unsigned short uint16;

typedef char int8;
typedef unsigned char uint8;

typedef unsigned size_t;

#else

#include <stddef.h>
#include <stdint.h>

typedef int64_t int64;
typedef uint64_t uint64;

typedef int32_t int32;
typedef uint32_t uint32;

typedef int16_t int16;
typedef uint16_t uint16;

typedef int8_t int8;
typedef uint8_t uint8;

#endif

#endif //PIOS_INTEGER_H
