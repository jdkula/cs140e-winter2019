;@ based on dwelch's start for bootloader.

.globl _start
_start:
    b skip
skip:
    mov sp,#0x08000000
    bl vmm_main
hang: b hang
