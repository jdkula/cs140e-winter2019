/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: mem-access.c [ARM Implementation]
 * --------------
 * Implements get32 and put32 in terms of
 * asm-defined functions that GCC won't optimize.
 */

#include "mem-access.h"

/**
 * Places [v]alues of varying widths in memory at the given [addr].
 * These versions of the functions link to the assembly seen in the .s files.
 */
void PUT32(uint32_t addr, uint32_t v);

void PUT16(uint32_t addr, uint32_t v);

void PUT8(uint32_t addr, uint32_t v);

/** Gets the value stored at a given [addr]. */
uint32_t GET32(uint32_t addr);

void put32(void* addr, uint32_t data) {
    PUT32((uint32_t) addr, data);
}

uint32_t get32(void* addr) {
    return GET32((uint32_t) addr);
}