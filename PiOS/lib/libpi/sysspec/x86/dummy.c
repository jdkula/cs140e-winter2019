/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: dummy.c [X86 Implementation]
 * --------------
 * Implements a do-nothing function used to delay operations.
 */

#include "rpi.h"

// We don't need to wait in x86-compiled projects, so we'll just let the compiler take this away...
void dummy(uint32_t unused) {
    // No-op
}