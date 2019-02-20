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

uint32_t setBit(uint32_t* addr, uint8_t bitNum, uint32_t value) {
    value = (value) & 0b1U; // ensure value is a single bit.

    uint32_t currentValue = get32(addr);
    uint32_t bitValue = value << bitNum;

    currentValue = (currentValue & ~bitValue) | bitValue;

    put32(addr, currentValue);
    return currentValue;
}

uint8_t getBit(uint32_t* addr, uint8_t bitNum) {
    uint32_t currentValue = get32(addr);

    return (uint8_t) ((currentValue >> bitNum) & 1U);
}