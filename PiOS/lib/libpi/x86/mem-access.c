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
#include "../include-common/mem-access.h"

/** Defines the max number of unique addresses that can store a value. */
#define MEMORY_CHUNKS 65565

/**
 * Defines a memory entry;
 * addr: Where in the Raspberry Pi's memory the value would have gone.
 * val: The value stored there.
 */
typedef struct {
    void* addr;
    uint32 val;
} mem_t;

/** The representation of memory. */
mem_t MEM[MEMORY_CHUNKS];
uint32 chunksUsed = 0;

/** Prints each read and write so solutions can be compared across students. */
static void print_write(mem_t* m) {
    printf("\tWRITE:addr=%p, val=%u\n", m->addr, m->val);
}

static void print_read(mem_t* m) {
    printf("\tREAD:addr=%p, val=%u\n", m->addr, m->val);
}

void put32(void* addr, uint32 data) {
    if (chunksUsed == MEMORY_CHUNKS) {
        fprintf(stderr, "Error: Out of Memory!");
        exit(137);
    }

    // Search through the array, searching for something already stored.
    for (uint32 i = 0; i < chunksUsed; i++) {
        if (MEM[i].addr == addr) {
            MEM[i].val = data;
            print_write(MEM + i);
            return;
        }
    }

    // Otherwise, go ahead and create a new entry.
    MEM[chunksUsed].addr = addr;
    MEM[chunksUsed].val = data;
    print_write(MEM + chunksUsed);
    chunksUsed++;

}

uint32 get32(void* addr) {

    // Search to see if we've already stored a value for this address.
    for (uint32 i = 0; i < chunksUsed; i++) {
        if (MEM[i].addr == addr) {
            print_read(MEM + i);
            return MEM[i].val;
        }
    }

    // If not, create a new chunk and initialize it randomly.
    MEM[chunksUsed].addr = (void*) addr;
    MEM[chunksUsed].val = random();
    //print_write(MEM + chunksUsed);
    chunksUsed++;

    print_read(MEM + chunksUsed - 1);
    return MEM[chunksUsed - 1].val;
}

// Get Program Counter
uint32 GETPC() {
    return 0; // TODO: Unsure how to do this...
}

/** Our current x86 test harness doesn't support branching, so this is a no-op. */
void BRANCHTO(uint32 addr) {
    return; // No-op
}