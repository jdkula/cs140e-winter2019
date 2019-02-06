/*
 * use interrupts to implement a simple statistical profiler.
 *	- interrupt code is a replication of ../timer-int/timer.c
 *	- you'll need to implement kmalloc so you can allocate 
 *	  a histogram table from the heap.
 *	- implement functions so that given a pc value, you can increment
 *	  its associated count
 */
#include "rpi.h"
#include "timer-interrupt.h"


/**********************************************************************
 * trivial kmalloc:  
 *	- do first.  check that its output makes sense!
 */

// useful for rounding up.   e.g., roundup(n,8) to roundup <n> to 8 byte
// alignment.
#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))

extern char __heap_start__;
extern char __bss_start__;
extern char __bss_end__;

volatile char* bss_end;
volatile char* bss_start;
volatile char* heap_start;
volatile char* const text_start = (void*) 0x8000;

/*
 * return a memory block of at least size <nbytes>
 *	- use to allocate gprof code histogram table.
 *	- note: there is no free, so is trivial.
 * 	- should be just a few lines of code.
 */
void *kmalloc(unsigned nbytes) {
  volatile void* here = heap_start;
  heap_start += nbytes;
  return (void*) here;
}

/*
 * one-time called before kmalloc to setup heap.
 * 	- should be just a few lines of code.
 */
void kmalloc_init(void) {
  bss_start = &__bss_start__;
  bss_end = &__bss_end__;
  heap_start = &__heap_start__;
  //printk("bss start/end | heap = %p/%p | %p\n", bss_start, bss_end, heap_start);
  //for(volatile char* p = bss_start; p < heap_start; p++) {
  //  *p = 0;
  //}
}


unsigned* gprof_table;
unsigned num_entries;

/***************************************************************************
 * gprof implementation:
 *	- allocate a table with one entry for each instruction.
 *	- gprof_init(void) - call before starting.
 *	- gprof_inc(pc) will increment pc's associated entry.
 *	- gprof_dump will print out all samples.
 */

// allocate table.
//    few lines of code
static void gprof_init(void) {
  kmalloc_init();
  unsigned size = roundup((bss_start - text_start), 4);
  gprof_table = kmalloc(size);
  num_entries = size / 4;

  printk("Table is at %p, of size %u\n", gprof_table, num_entries);

  for(unsigned* p = gprof_table; p < (gprof_table + num_entries); p++) {
    *p = 0;
    //printk("Clearing %p. \n", p);
  }
}

unsigned char skip_flag = 0;

// increment histogram associated w/ pc.
//    few lines of code
static void gprof_inc(unsigned pc) {
  //printk("Incrementing %p.\n", pc);
  if(skip_flag == 1) {
    //printk("Skipping.\n");
    return;
  }
  if(pc < (unsigned) text_start || pc > (unsigned) bss_end) {
    //printk("Out of bounds instruction: %p\n", pc);
    return;
  }

  unsigned idx = (pc - (unsigned) text_start) / 4;

  //printk("++%u++", idx);

  gprof_table[idx] = gprof_table[idx] + 1;
}

// print out all samples whose count > min_val
//
// make sure sampling does not pick this code up!
static void gprof_dump(unsigned min_val) {
  skip_flag = 1;
  dsb();
  for(unsigned i = 0; i < num_entries; i++) {
    if(gprof_table[i] > min_val) {
      printk("%p -> %u\n", text_start + (i * 4), gprof_table[i]);
    }
  }
  skip_flag = 0;
  dsb();
}


/***********************************************************************
 * timer interrupt code from before, now calls gprof update.
 */
// Q: if you make not volatile?
static volatile unsigned cnt;
static volatile unsigned period;

// client has to define this.
void int_handler(unsigned pc) {
	unsigned pending = RPI_GetIRQController()->IRQ_basic_pending;

	// if this isn't true, could be a GPU interrupt: just return.
	if((pending & RPI_BASIC_ARM_TIMER_IRQ) == 0)
		return;

        /* 
	 * Clear the ARM Timer interrupt - it's the only interrupt we have
         * enabled, so we want don't have to work out which interrupt source
         * caused us to interrupt 
	 *
	 * Q: if we delete?
	 */
        RPI_GetArmTimer()->IRQClear = 1;
	cnt++;

        gprof_inc(pc);

	static unsigned last_clk = 0;
	unsigned clk = timer_get_time();
	period = last_clk ? clk - last_clk : 0;
	last_clk = clk;
	
	// Q: if we put a print statement?
}

// trivial program to test gprof implementation.
// 	- look at output: do you see weird patterns?
void notmain() {
	uart_init();
	
	printk("about to install handlers\n");
        install_int_handlers();

	printk("setting up timer interrupts\n");
	// Q: if you change 0x100?
	timer_interrupt_init(0x10);

	// could combine some of these.
        gprof_init();

	// Q: if you don't do?
	printk("gonna enable ints globally!\n");
  	system_enable_interrupts();
	printk("enabled!\n");

	// enable_cache(); 	// Q: what happens if you enable cache?
        unsigned iter = 0;
        while(cnt<2000) {
                printk("iter=%d: cnt = %d, period = %dusec, %x\n",
                                iter,cnt, period,period);
                iter++;
                if(iter % 10 == 0)
                        gprof_dump(2);
        }

        gprof_dump(2);
	clean_reboot();
}
