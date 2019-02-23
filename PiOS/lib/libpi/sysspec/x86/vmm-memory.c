/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: vmm-memory.c
 * --------------
 * Provides memory commands for the VMM.
 */

#include "uart.h"
#include "vmm/pi-vmm-ops.h"
#include "mem-access.h"
#include "mem-barrier.h"
#include "x86/mem-x86-extensions.h"

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

void put32(void* addr, uint32_t data) {
    put_uint(OP_WRITE32);
    put_uint((unsigned) addr);
    put_uint((unsigned) data);
    mem_t info = {.val = data, .addr = addr};
    on_write(info);
}

uint32_t get32(void* addr) {
    put_uint(OP_READ32);
    put_uint((unsigned) addr);
    uint32_t data = get_uint();
    mem_t info = {.val = data, .addr = addr};
    on_read(info);
    return data;
}

// Get Program Counter
uint32_t GETPC() {
    return 0; // TODO: Unsure how to do this...
}

/** Our current x86 test harness doesn't support branching, so this is a no-op. */
void BRANCHTO(uint32_t addr) {
    return; // No-op
}