/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: integer.h [X86 Implementation]
 * --------------
 * Provides a consistent definition of ints
 * of various signededness and widths across platforms.
 *
 * We use stdint.h on x86 for maximum compatibility.
 */

#ifndef PIOS_INTEGER_H
#define PIOS_INTEGER_H

#include <stdint.h>
#include <stddef.h>

typedef int64_t int64;
typedef uint64_t uint64;

typedef int32_t int32;
typedef uint32_t uint32;

typedef int16_t int16;
typedef uint16_t uint16;

typedef int8_t int8;
typedef uint8_t uint8;

#endif //PIOS_INTEGER_H
