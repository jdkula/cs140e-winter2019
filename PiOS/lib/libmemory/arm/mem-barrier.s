/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: mem-barrier.s
 * --------------
 * Implements memory barriers in using ARM cache
 *
 */

// https://github.com/raspberrypi/firmware/wiki/Accessing-mailboxes
.globl mb
mb:
    mov r0, #0
    mcr p15, 0, r0, c7, c5, 0   // Invalidate instruction cache
    mcr p15, 0, r0, c7, c5, 6   // Invalidate BTB
    mcr p15, 0, r0, c7, c10, 4  // Drain write buffer
    mcr p15, 0, r0, c7, c5, 4   // Prefetch flush
    bx lr

// Source: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0360f/I1014942.html
/*
 * Data memory barrier
 *
 * No memory access after the DMB can run until
 * all memory accesses before it have completed
 */
.globl dmb
dmb:
    mcr p15, 0, r0, c7, c10, 5
    bx lr

/*
 * Data synchronisation barrier
 *
 * No instruction after the DSB can run until
 * all instructions before it have completed
 */
.globl dsb
dsb:
    mcr p15, 0, r0, c7, c10, 4
    bx lr
