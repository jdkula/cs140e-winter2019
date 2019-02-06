/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: mem-x86-extensions.c [X86 Only]
 * --------------
 * Provides a way to interface with the faux memory
 * module provided on x86 architectures.
 */

#include "x86/mem-x86-extensions.h"

/** Comparison function for mem_t. Sorts by address, then value. The void*s are casted to (mem_t*). */
int mem_compare(const void* a, const void* b) {
    const mem_t* mem1 = (mem_t*) a;
    const mem_t* mem2 = (mem_t*) b;

    if(mem1->addr == mem2->addr) {
        return (mem1->val > mem2->val) - (mem1->val < mem2->val);
    }
    return (mem1->addr > mem2->addr) - (mem1->addr < mem2->addr);
}