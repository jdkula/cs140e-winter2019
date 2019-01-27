;@ based on dwelch's start for bootloader.

.globl _start
_start:
    mov sp,#0x8000
    bl module_main
hang: b hang
