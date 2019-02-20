@empty stub routines.  use these, or make your own.

.globl test_csave
test_csave:
  sub r0, r0, #64
  str r0, [r0, #0]
  str r1, [r0, #4]
  str r2, [r0, #8]
  str r3, [r0, #12]
  str r4, [r0, #16]
  str r5, [r0, #20]
  str r6, [r0, #24]
  str r7, [r0, #28]
  str r8, [r0, #32]
  str r9, [r0, #36]
  str r10, [r0, #40]
  str r11, [r0, #44]
  str r12, [r0, #48]
  str r13, [r0, #52]
  str r14, [r0, #56]

  mrs r1, cpsr
  str r1, [r0, #60]
	bx lr

.globl test_csave_stmfd
test_csave_stmfd:
	bx lr

.globl rpi_get_cpsr
rpi_get_cpsr:
  mrs r0, cpsr
	bx lr

.globl rpi_cswitch
rpi_cswitch:
  sub r13, r13, #64  @ Lower the stack
  str r0, [r13, #0]  @ Save everything [except R13/SP]
  str r1, [r13, #4]
  str r2, [r13, #8]
  str r3, [r13, #12]
  str r4, [r13, #16]
  str r5, [r13, #20]
  str r6, [r13, #24]
  str r7, [r13, #28]
  str r8, [r13, #32]
  str r9, [r13, #36]
  str r10, [r13, #40]
  str r11, [r13, #44]
  str r12, [r13, #48]
  str r14, [r13, #56]

  mrs r2, cpsr       @ Save CPSR
  str r2, [r13, #60]

  str r13, [r0]      @ Save SP to old


  ldr r13, [r1]  @ Load SP from new

  ldr r0, [r13, #60]  @ Restore CPSR forom new
  msr cpsr, r0

  ldr r0, [r13, #0]   @ Load from stack
  ldr r1, [r13, #4]
  ldr r2, [r13, #8]
  ldr r3, [r13, #12]
  ldr r4, [r13, #16]
  ldr r5, [r13, #20]
  ldr r6, [r13, #24]
  ldr r7, [r13, #28]
  ldr r8, [r13, #32]
  ldr r9, [r13, #36]
  ldr r10, [r13, #40]
  ldr r11, [r13, #44]
  ldr r12, [r13, #48]
  ldr r14, [r13, #56]

  add r13, r13, #64   @ Reset stack

	bx lr               @ Continue

@ [Make sure you can answer: why do we need to do this?]
@
@ use this to setup each thread for the first time.
@ setup the stack so that when cswitch runs it will:
@	- load address of <rpi_init_trampoline> into LR
@	- <code> into r1, 
@	- <arg> into r0
@ 
.globl rpi_init_trampoline
rpi_init_trampoline:
  @ bl rip_debug
	blx r1
  bl rpi_exit
