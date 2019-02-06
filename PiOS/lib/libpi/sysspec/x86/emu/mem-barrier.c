/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: mem-barrier.c [X86 Implementation]
 * --------------
 * Since our faux memory model already has sequential
 * consistency, we just implement event handlers here
 * so that they can be taken into account in test harnesses.
 */

#include "mem-barrier.h"
#include "x86/mem-x86-extensions.h"

memory_barrier_handler_t mem_on_barrier = 0;

void data_memory_barrier(void) {
    if(mem_on_barrier != 0) mem_on_barrier();
}

void data_sync_barrier(void) {
    if(mem_on_barrier != 0) mem_on_barrier();
}