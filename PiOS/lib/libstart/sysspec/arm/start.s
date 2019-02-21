// To keep this in the first portion of the binary.
.section ".text.boot"

.globl _start
_start:
    b _cstart
