/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: dummy.s [ARM Implementation]
 * --------------
 * Implements a do-nothing function used to delay operations.
 */

/**
 * Does absolutely nothing, but won't be compiled
 * away by gcc. Used to cause the system to wait.
 */
.globl dummy
dummy:
    bx lr
