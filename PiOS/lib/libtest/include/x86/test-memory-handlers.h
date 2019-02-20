//
// Created by jonak on 1/28/19.
//

#ifndef PIOS_TEST_MEMORY_HANDLERS_H
#define PIOS_TEST_MEMORY_HANDLERS_H

#include <x86/mem-x86-extensions.h>
#include <integer.h>

void test_default_write_handler(const mem_t* m);

void test_default_read_handler(const mem_t* m);

void test_default_barrier_handler();

void test_default_commit_handler(const mem_t* list, uint32_t size, uint8_t commit_type);

#endif //PIOS_TEST_MEMORY_HANDLERS_H
