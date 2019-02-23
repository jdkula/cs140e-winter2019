/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: boot-messages.h
 * --------------
 * Provides the messages used in the process of
 * transmitting a boot image to the bootloader.
 */

#ifndef PIOS_BOOT_MESSAGES_H
#define PIOS_BOOT_MESSAGES_H

enum {
    /** Location the sent payload gets stored. */
    ARMBASE = 0x8000,

    /** Start Of Header constant */
    SOH = 0xCCU,

    /** Error code for a bad checksum on either side. */
    BAD_CKSUM = 0x1,

    /** Error code for bad start (?) TODO: Why? */
    BAD_START,

    /** Error code for a bad end (?) TODO: Understand this */
    BAD_END,

    /** Error code sent when the recieved payload's size doesn't match what was telegraphed. */
    SIZE_MISMATCH,

    /** Acknowledgement of Receipt */
    ACK,

    /** Acknowledgement with error -- indicates that the process should start over. */
    NAK,

    /** Indicates the End of Transmission */
    EOT
};

#endif //PIOS_BOOT_MESSAGES_H
