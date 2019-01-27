/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: mem-barrier.s [ARM Implementation]
 * --------------
 * Implements memory barriers in using ARM cache directives.
 *
 * Source: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0360f/I1014942.html
 */

/*
 * Data memory barrier
 *
 * No memory access after the DMB can run until
 * all memory accesses before it have completed.
 */
.globl data_memory_barrier
data_memory_barrier:
    mcr p15, 0, r0, c7, c10, 5
    bx lr

/*
 * Data synchronisation barrier
 *
 * No instruction after the DSB can run until
 * all instructions before it have completed.
 */
.globl data_sync_barrier
data_sync_barrier:
    mcr p15, 0, r0, c7, c10, 4
    bx lr
