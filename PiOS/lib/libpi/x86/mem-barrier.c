/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: mem-barrier.c [X86 Implementation]
 * --------------
 * Since our faux memory model already has sequential
 * consistency, we just have no-op functions here so
 * there are no linker errors. The compiler should just
 * completely eliminate these functions during compilation.
 */

#include "mem-barrier.h"

void data_memory_barrier(void) {
    // No-op
}

void data_sync_barrier(void) {
    // No-op
}