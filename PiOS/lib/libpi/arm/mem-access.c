/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: mem-access.c [ARM Implementation]
 * --------------
 * Implements get32 and put32 in terms of
 * asm-defined functions that GCC won't optimize.
 */

#include "../include-common/mem-access.h"

/**
 * Places [v]alues of varying widths in memory at the given [addr].
 * These versions of the functions link to the assembly seen in the .s files.
 */
void PUT32(uint32 addr, uint32 v);

void PUT16(uint32 addr, uint32 v);

void PUT8(uint32 addr, uint32 v);

/** Gets the value stored at a given [addr]. */
uint32 GET32(uint32 addr);

void put32(void* addr, uint32 data) {
    PUT32((uint32) addr, data);
}

uint32 get32(void* addr) {
    return GET32((uint32) addr);
}