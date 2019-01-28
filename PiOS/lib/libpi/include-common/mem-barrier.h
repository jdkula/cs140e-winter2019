/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: mem-barrier.h
 * --------------
 * Provides an interface for the processor's
 * memory barriers.
 */

/**
 * Data memory barrier
 *
 * No memory access after the DMB can run until
 * all memory accesses before it have completed.
 * (Preserves apparent write order).
 *
 * DMB is a subset of DSB. Source: https://stackoverflow.com/questions/23105052/is-there-a-need-for-dmb-if-we-are-using-dsb
 */
void data_memory_barrier(void);

/**
 * Data synchronisation barrier
 *
 * No instruction after the DSB can run until
 * all instructions before it have completed.
 */
void data_sync_barrier(void);
