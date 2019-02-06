/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: tests.h
 * --------------
 * Provides a testing harness for x86.
 * Currently helps debug memory accesses.
 */

#ifndef PIOS_TESTS_H
#define PIOS_TESTS_H

#include <x86/mem-x86-extensions.h>

void test_mem_init();
const mem_t* get_raw_memory();

typedef void (*on_commit_t)(const mem_t* committed, uint32 size, uint8 commit_type);

extern memory_operation_handler_t test_on_read;
extern memory_operation_handler_t test_on_write;
extern memory_barrier_handler_t test_on_barrier;

extern on_commit_t test_on_commit;

enum {
    COMMIT_NONE,
    COMMIT_READ,
    COMMIT_WRITE
};


#endif //PIOS_TESTS_H
