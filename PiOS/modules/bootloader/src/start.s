;@ based on dwelch's start for bootloader.

.globl _start
_start:
    b skip
;@ Why? .space 0x200000-0x8004,0
skip:
    mov sp,#0x08000
    bl module_main
hang: b hang
