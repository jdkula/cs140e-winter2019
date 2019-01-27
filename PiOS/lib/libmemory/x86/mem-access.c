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
    uint32 val;
} mem_t;

mem_t MEM[MEMORY_CHUNKS];
uint32 chunksUsed = 0;

void setBit(uint32* reg, uint8 bitNum, uint8 value) {
    uint32 currentValue = get32(reg);
    uint32 bitValue = (value << bitNum) & (1U << bitNum);

    currentValue = (currentValue & ~bitValue) | bitValue;

    put32(reg, currentValue);
}

uint8 getBit(uint32* reg, uint8 bitNum) {
    uint32 currentValue = get32(reg);

    return (uint8) ((currentValue >> bitNum) & 1U);
}

// don't change print_write/print_read so we can compare to everyone.
static void print_write(mem_t *m) {
    printf("\tWRITE:addr=%p, val=%u\n", m->addr, m->val);
}
static void print_read(mem_t *m) {
    printf("\tREAD:addr=%p, val=%u\n", m->addr, m->val);
}

void put32(void* addr, uint32 data) {

    for(uint32 i = 0; i < chunksUsed; i++) {
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

uint32 get32(void* addr) {

    for(uint32 i = 0; i < chunksUsed; i++) {
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

// *(uint32 *)addr = v;
void PUT32(uint32 addr, uint32 v) {
    return put32((void*) addr, v);
}

// *(uint16 *)addr = v;
void PUT16(uint32 addr, uint32 v) {
    return put32((void*) addr, v);
}

// *(uint8 *)addr = v;
void PUT8(uint32 addr, uint32 v) {
    return put32((void*) addr, v);
}

// *(uint32 *)addr
uint32 GET32(uint32 addr) {
    return get32((void*) addr);
}

#pragma GCC diagnostic pop

// Get Program Counter
uint32 GETPC() {
    return 0; // TODO: Unsure how to do this...
}

void BRANCHTO(uint32 addr) {
    // TODO
}