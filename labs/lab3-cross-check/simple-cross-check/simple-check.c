/*
 * Simple tracing memory implementation.  Used for cross-checking.
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "rpi.h"
#include "simple-check.h"

static int pin_gen(unsigned *out) {
        static unsigned u = 0;
        if(u > 70) {
                u = 0;
                return 0;
        }
        *out = (u < 64) ? u : random();
        u++;
        return 1;
}

gen_t xc_get_pin_gen(void) {
        return pin_gen;
}

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

#define MEMORY_CHUNKS 1024

mem_t MEM[MEMORY_CHUNKS];
unsigned int chunksUsed = 0;

// don't change print_write/print_read so we can compare to everyone.
static void print_write(mem_t *m) {
	printf("\tWRITE:addr=%p, val=%u\n", m->addr, m->val);
}
static void print_read(mem_t *m) {
	printf("\tREAD:addr=%p, val=%u\n", m->addr, m->val);
}

/********************************************************************
 * implement these two functions.
 */

void put32(volatile void* addr, unsigned int data) {

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

unsigned int get32(volatile void* addr) {

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

/***********************************************************************
 * harnesses to run checked code.
 */
void xc_run_fn_iu(const char *A, int (*a)(unsigned), gen_t g) {
	unsigned in;
	while(g(&in)) {
		printf("-----------------------------------------\n");
		printf("going to check <%s(%d)>\n", A, in);
		int r1 = a(in);
		printf("returned=%d\n", r1);
	}
}

void xc_run_fn_vv_once(const char *A, void (*a)(void)) {
	printf("--------------------------------------------\n");
	printf("going to check <%s>\n", A);
	a();
	printf("returned\n");
}

static int pin_gen(unsigned *out) {
        static unsigned u = 0;
        if(u > 70) {
                u = 0;
                return 0;
        }
        *out = (u < 64) ? u : random();
        u++;
        return 1;
}

gen_t xc_get_pin_gen(void) {
        return pin_gen;
}
