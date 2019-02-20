/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: vmm-memory.c
 * --------------
 * Provides memory commands for the VMM.
 */

#include "uart.h"
#include "vmm/pi-vmm-ops.h"
#include "mem-barrier.h"
#include "x86/mem-x86-extensions.h"

memory_barrier_handler_t mem_on_barrier = NULL;

void data_memory_barrier(void) {
    put_uint(OP_DMB);
    uint32 ack = get_uint();
    if(mem_on_barrier != NULL) {
        mem_on_barrier();
    }
}


void data_sync_barrier(void) {
    put_uint(OP_DSB);
    uint32 ack = get_uint();
    if(mem_on_barrier != NULL) {
        mem_on_barrier();
    }
}