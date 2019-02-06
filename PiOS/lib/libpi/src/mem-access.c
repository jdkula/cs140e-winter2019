/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: mem-access.c [Common Implementation]
 * --------------
 * Implements setBit and getBit, which depend
 * on the architecture-specific implementations
 * of get32 and put32.
 */

#include <integer.h>
#include "mem-access.h"

uint32 setBit(uint32* addr, uint8 bitNum, uint32 value) {
    value = (value) & 0b1U; // ensure value is a single bit.

    uint32 currentValue = get32(addr);
    uint32 bitValue = value << bitNum;

    currentValue = (currentValue & ~bitValue) | bitValue;

    put32(addr, currentValue);
    return currentValue;
}

uint8 getBit(uint32* addr, uint8 bitNum) {
    uint32 currentValue = get32(addr);

    return (uint8) ((currentValue >> bitNum) & 1U);
}