//
// Created by jonak on 1/23/19.
//

#include "helper.h"
#include "rpi.h"

void setBit(volatile unsigned int* reg, unsigned char bitNum, unsigned char value) {
    unsigned int currentValue = get32(reg);
    unsigned int bitValue = (value << bitNum) & (1U << bitNum);

    currentValue = (currentValue & ~bitValue) | bitValue;

    put32(reg, currentValue);
}

unsigned char getBit(volatile unsigned int* reg, unsigned char bitNum) {
    unsigned int currentValue = get32(reg);

    return (unsigned char) ((currentValue >> bitNum) & 1U);
}