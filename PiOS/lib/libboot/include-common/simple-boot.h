/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: simple-boot.h
 * --------------
 * Provides prototypes and constants used in the
 * simple bootloader.
 */

#include <integer.h>

enum {
    /** Location the sent payload gets stored. */
    ARMBASE = 0x8000,

    /** Start Of Header constant */
    SOH = 0x12345678,

    /** Error code for a bad checksum on either side. */
    BAD_CKSUM = 0x1,

    /** Error code for bad start (?) TODO: Why? */
    BAD_START,

    /** Error code for a bad end (?) TODO: Understand this */
    BAD_END,

    /** Error code sent when the recieved payload's size doesn't match what was telegraphed. */
    SIZE_MISMATCH,

    /** Acknowledgement of Receipt */
    ACK,   // client ACK

    /** Acknowledgement with error -- indicates that the process should start over. */
    NAK,

    /** Indicates the End of Transmission */
    EOT
};

/**
 * crc32 implementation.
 * Moves around the above hash table according to
 * each byte of an input [buf] of a given [size].
 */
uint32 crc32(const void* buf, uint32 size);
