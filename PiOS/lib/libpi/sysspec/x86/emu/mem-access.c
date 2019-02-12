/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: mem-access.c [X86 Implementation]
 * --------------
 * Implements get32 and put32 in terms of
 * a faux (and inefficient!) memory that's
 * hooked in with a test harness.
 */

#include <stdlib.h>
#include <stdio.h>
#include "mem-access.h"
#include "x86/mem-x86-extensions.h"

/** The faux representation of memory. */
mem_t ram_emulated[MEMORY_CHUNKS];
uint32 chunksUsed = 0;

/** Event handler storage. A value of NULL disables the handler. */
memory_operation_handler_t mem_on_write = NULL;
memory_operation_handler_t mem_on_read = NULL;

/** Invokes the on-write handler if it's defined. */
static void on_write(mem_t info) {
    if (mem_on_write != NULL) {
        mem_on_write(info);
    }
}

/** Invokes the on-read handler if it's defined. */
static void on_read(mem_t info) {
    if (mem_on_read != NULL) {
        mem_on_read(info);
    }
}

void put32(void* addr, uint32 data) {
    if (chunksUsed == MEMORY_CHUNKS) {
        fprintf(stderr, "Error: Out of Memory!");
        exit(137);
    }

    // Search through the array, searching for something already stored.
    for (uint32 i = 0; i < chunksUsed; i++) {
        if (ram_emulated[i].addr == addr) {
            ram_emulated[i].val = data;
            on_write(ram_emulated[i]);
            return;
        }
    }

    // Otherwise, go ahead and create a new entry.
    ram_emulated[chunksUsed].addr = addr;
    ram_emulated[chunksUsed].val = data;
    on_write(ram_emulated[chunksUsed]);
    chunksUsed++;
}

uint32 get32(void* addr) {
    // Search to see if we've already stored a value for this address.
    for (uint32 i = 0; i < chunksUsed; i++) {
        if (ram_emulated[i].addr == addr) {
            on_read(ram_emulated[i]);
            return ram_emulated[i].val;
        }
    }

    // If not, create a new chunk and initialize it randomly.
    ram_emulated[chunksUsed].addr = (void*) addr;
    ram_emulated[chunksUsed].val = random();
    //print_write(ram_emulated + chunksUsed);
    chunksUsed++;

    on_read(ram_emulated[chunksUsed - 1]);
    return ram_emulated[chunksUsed - 1].val;
}

// Get Program Counter
uint32 GETPC() {
    return 0; // TODO: Unsure how to do this...
}

/** Our current x86 test harness doesn't support branching, so this is a no-op. */
void BRANCHTO(uint32 addr) {
    return; // No-op
}