;@ based on dwelch's start for bootloader.

.globl _start
_start:
    mov sp,#0x8000000
    mov fp,#0
    b _cstart