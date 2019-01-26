;@ based on dwelch's start for bootloader.

.globl _start
_start:
    b skip
skip:
    mov sp,#0x08000
    bl notmain
hang: b hang
