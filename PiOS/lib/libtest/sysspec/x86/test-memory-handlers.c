//
// Created by jonak on 1/28/19.
//

#include <x86/mem-x86-extensions.h>
#include <stdio.h>
#include "x86/test-memory.h"
#include "x86/test-memory-handlers.h"


void test_default_write_handler(const mem_t* m) {
    printf("\tWRITE:addr=%p, val=%u\n", m->addr, m->val);
}

void test_default_read_handler(const mem_t* m) {
    printf("\tREAD:addr=%p, val=%u\n", m->addr, m->val);
}

void test_default_barrier_handler() {
    printf("\tBARRIER\n");
}

void test_default_commit_handler(const mem_t* list, uint32 size, uint8 commit_type) {
    printf("\tCOMMIT\n");
    for (const mem_t* this = list; this < (list + size); this++) {
        if (commit_type == COMMIT_READ) {
            test_default_read_handler(this);
        } else {
            test_default_write_handler(this);
        }
    }
}