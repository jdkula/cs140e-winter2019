//
// Created by jonak on 1/28/19.
//


#include <stdio.h>
#include <mem-x86-extensions.h>
#include <mem-access.h>
#include <test-memory.h>
#include <test-memory-handlers.h>
#include <mem-barrier.h>

int main() {
    test_mem_init();

    test_on_commit = test_default_commit_handler;
    test_on_barrier = test_default_barrier_handler;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-conversion"
    get32(0x0);
    get32(0x1);
    get32(0x2);
    put32(0x0, 0x1);
    put32(0x0, 0x2);
    put32(0x1, 0x2);
    get32(0x0);
#pragma GCC diagnostic pop

    data_sync_barrier();
}

