/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: mem-x86-extensions.h [X86 Only]
 * --------------
 * Provides a way to interface with the faux memory
 * module provided on x86 architectures.
 */

#ifndef PIOS_MEM_X86_EXTENSIONS_H
#define PIOS_MEM_X86_EXTENSIONS_H

#include <integer.h>

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

/** Event handler typedefs */
typedef void (*memory_operation_handler_t)(mem_t m);
typedef void (*memory_barrier_handler_t)();

/** Event handler locations. Set to a function to call it on that event; set to NULL/0 to disable. */
extern memory_operation_handler_t mem_on_write;
extern memory_operation_handler_t mem_on_read;

extern memory_barrier_handler_t mem_on_barrier;

/** The raw RAM representation. */
extern mem_t ram_emulated[MEMORY_CHUNKS];

/** Provides comparison between two mem_t's. The void*s are casted to (mem_t*). */
int mem_compare(const void* a, const void* b);

#endif //PIOS_MEM_X86_EXTENSIONS_H
