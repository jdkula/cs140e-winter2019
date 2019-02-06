//
// Created by jonak on 1/28/19.
//


#include <stdio.h>
#include <x86/mem-x86-extensions.h>
#include <mem-access.h>
#include <x86/test-memory.h>
#include <x86/test-memory-handlers.h>
#include <mem-barrier.h>
#include <pios-macros.h>
#include <vmm/x86/vmm.h>

extern int notmain();

int main() {
    test_mem_init();
    vmm_init();

    test_on_commit = test_default_commit_handler;
    test_on_barrier = test_default_barrier_handler;

    notmain();

    data_sync_barrier();
}

