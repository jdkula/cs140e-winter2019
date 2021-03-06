/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: mem-access.h
 * --------------
 * Provides an alternative interface for
 * accessing memory that is A) unlikely
 * to be unfavorably optimized by the compiler,
 * and B) easy to swap in with an x86 test harness.
 */

#ifndef PIOS_MEM_ACCESS_H
#define PIOS_MEM_ACCESS_H

#include <integer.h>

/** Gets the current program counter (returns 0 on x86) */
uint32_t GETPC();

/** Moves the current instruction pointer to the given address. */
void BRANCHTO(uint32_t addr);

/**
 * Sets a single bit (at [bitNum], counting from least
 * to most significant bit), to a given [value], in
 * the area of memory pointed to by [addr].
 * Returns the output value, so this can be used
 * as part of expressions.
 */
uint32_t setBit(uint32_t* addr, uint8_t bitNum, uint32_t value);

/**
 * Gets a single bit (at [bitNum], counting from least
 * to most significant bit) in the area of memory
 * pointed to by [addr].
 */
uint8_t getBit(uint32_t* addr, uint8_t bitNum);

/** Puts the given [data] at the given [addr]. */
void put32(void* addr, uint32_t data);

/** Gets whatever data is at the given [addr]. */
uint32_t get32(void* addr);


#endif //PIOS_MEM_ACCESS_H
