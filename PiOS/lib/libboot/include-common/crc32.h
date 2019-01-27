/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: crc32.h
 * --------------
 * Provides the prototype for the crc32 function used in the bootloader.
 */

#include <integer.h>

/**
 * crc32 implementation.
 * Moves around the above hash table according to
 * each byte of an input [buf] of a given [size].
 */
uint32 crc32(const void* buf, uint32 size);
