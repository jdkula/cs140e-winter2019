//
// Created by jonak on 1/23/19.
//

#include <stdlib.h>
#include <stdio.h>
#include "mem-access.h"

#define MEMORY_CHUNKS 1024

/*
 * simplistic memory:
 *	- all loads/stores are 32 bits.
 * 	- read returns value of last write, or random() if none.
 *	- load and store prints the addr,val
 */
typedef struct {
    void *addr;
    unsigned val;
} mem_t;

mem_t MEM[MEMORY_CHUNKS];
unsigned int chunksUsed = 0;

void setBit(unsigned int* reg, unsigned char bitNum, unsigned char value) {
    unsigned int currentValue = get32(reg);
    unsigned int bitValue = (value << bitNum) & (1U << bitNum);

    currentValue = (currentValue & ~bitValue) | bitValue;

    put32(reg, currentValue);
}

unsigned char getBit(unsigned int* reg, unsigned char bitNum) {
    unsigned int currentValue = get32(reg);

    return (unsigned char) ((currentValue >> bitNum) & 1U);
}

// don't change print_write/print_read so we can compare to everyone.
static void print_write(mem_t *m) {
    printf("\tWRITE:addr=%p, val=%u\n", m->addr, m->val);
}
static void print_read(mem_t *m) {
    printf("\tREAD:addr=%p, val=%u\n", m->addr, m->val);
}

void put32(void* addr, unsigned int data) {

    for(unsigned int i = 0; i < chunksUsed; i++) {
        if(MEM[i].addr == addr) {
            MEM[i].val = data;
            print_write(MEM + i);
            return;
        }
    }

    MEM[chunksUsed].addr = (void*) addr;
    MEM[chunksUsed].val = data;
    print_write(MEM + chunksUsed);
    chunksUsed++;

}

unsigned int get32(void* addr) {

    for(unsigned int i = 0; i < chunksUsed; i++) {
        if(MEM[i].addr == addr) {
            print_read(MEM + i);
            return MEM[i].val;
        }
    }


    MEM[chunksUsed].addr = (void*) addr;
    MEM[chunksUsed].val = random();
    //print_write(MEM + chunksUsed);
    chunksUsed++;

    print_read(MEM + chunksUsed - 1);
    return MEM[chunksUsed - 1].val;
}

// *(unsigned *)addr = v;
void PUT32(unsigned addr, unsigned v) {
    return put32((void*) addr, v);
}

// *(unsigned short *)addr = v;
void PUT16(unsigned addr, unsigned v) {
    return put32((void*) addr, v);
}

// *(unsigned char *)addr = v;
void PUT8(unsigned addr, unsigned v) {
    return put32((void*) addr, v);
}

// *(unsigned *)addr
unsigned GET32(unsigned addr) {
    return get32((void*) addr);
}

// Get Program Counter
unsigned GETPC() {
    return 0; // TODO: Unsure how to do this...
}

void BRANCHTO(unsigned addr) {
    // TODO
}