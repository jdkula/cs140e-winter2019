#ifndef __RPI_THREAD_H__
#define __RPI_THREAD_H__

#include <integer.h>

typedef struct rpi_thread {
	struct rpi_thread *next;
	uint32 *sp;
	uint32 tid;
	uint32 stack[1024 * 8];
} rpi_thread_t;

// create a new thread.
rpi_thread_t *rpi_fork(void (*code)(void *arg), void *arg);

// exit current thread.
void rpi_exit(int exitcode);

// yield the current thread.
void rpi_yield(void);

// starts the thread system: nothing runs before.
// 	- <preemptive_p> = 1 implies pre-emptive multi-tasking.
void rpi_thread_start(int preemptive_p);

// pointer to the current thread.  
//	- note: if pre-emptive is enabled this can change underneath you!
rpi_thread_t *rpi_cur_thread(void);

uint32 rpi_get_cpsr(void);

// context-switch:
//      - after saving state, write value of sp to <sp_old>
//      - switch to <sp_new> and restore values.
// note:
// 	for this lab, we have the prototype returning an unsigned to help
// 	debugging, but it should be <void>
unsigned rpi_cswitch(uint32 **sp_old, uint32 **sp_new);

#endif
