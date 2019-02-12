/*
	Example of how to use interrupts to interleave two tasks: setting
	a LED smoothly to different percentage of brightness and periodically
	reading sonar distance.
	
	Without interrupts have to decide how to PWM led while you are 
	doing sonar.  Can easily get jagged results (flickering) if you
	don't do it often enough.  Manually putting in code to interleave
	is a pain.

	Current method to fix:
		1. non-interrupt code periodically measures distance
		and computes the off/on decisions in a vector 

		2. interrupt handler cycles through vector doing off/on based
		on it's values.

	You could do the other way: sonar measurements in the int handler,
	pwm in the main code.  Q: tradeoffs?
*/
#include "rpi.h"
#include "timer-interrupt.h"
#include "pwm.h"

#define ECHO 5
#define TRIG 20
#define LED  21

#define MAX_DIST 10

#define CYCLE_SIZE 1024

unsigned duty_cycle[CYCLE_SIZE];
unsigned duty_select = 0;

unsigned last_period = 0;

// useful timeout.
int timeout_read(int pin, int v, unsigned timeout) {
	unsigned start = timer_get_time();
	while(1) {
		if(gpio_read(pin) != v)
			return 1;
		if(timer_get_time() - start > timeout)
			return 0;
	}
}

void int_handler(unsigned pc) {
	/*
	   Can have GPU interrupts even though we just enabled timer: 
	   check for timer, ignore if not.

	   p 114:  bits set in reg 8,9
	   These bits indicates if there are bits set in the pending
	   1/2 registers. The pending 1/2 registers hold ALL interrupts
	   0..63 from the GPU side. Some of these 64 interrupts are
	   also connected to the basic pending register. Any bit set
	   in pending register 1/2 which is NOT connected to the basic
	   pending register causes bit 8 or 9 to set. Status bits 8 and
	   9 should be seen as "There are some interrupts pending which
	   you don't know about. They are in pending register 1 /2."
	 */
	volatile rpi_irq_controller_t *r = RPI_GetIRQController();
	if(r->IRQ_basic_pending & RPI_BASIC_ARM_TIMER_IRQ) {


    gpio_write(LED, duty_cycle[duty_select++]);
    duty_select %= CYCLE_SIZE;

        	/* 
		 * Clear the ARM Timer interrupt - it's the only interrupt 
		 * we have enabled, so we want don't have to work out which 
		 * interrupt source caused us to interrupt 
		 */
        	RPI_GetArmTimer()->IRQClear = 1;
	}
}

void notmain() {
        uart_init();

        pwm_compute(duty_cycle, CYCLE_SIZE, CYCLE_SIZE);

        install_int_handlers();

        timer_interrupt_init(0x1);

        gpio_set_output(LED);
        gpio_set_output(TRIG);
        gpio_set_input(ECHO);
        gpio_set_pulldown(ECHO);


	// do last!
  	system_enable_interrupts();  	// Q: comment out: what happens?

	printk("starting sonar!\n");

  gpio_set_pulldown(ECHO);

  gpio_set_input(ECHO);
  gpio_set_output(TRIG);


	// put your code here.  
	//
	// you'll need to:
	// 	1. init the device (pay attention to time delays here)
	//
	//	2. do a send (again, pay attention to any needed time 
	// 	delays)
	//
	//	3. measure how long it takes and compute round trip
	//	by converting that time to distance using the datasheet
	// 	formula
	//
	// 	4. use the code in gpioextra.h and then replace it with your
	//	own (using the broadcom pdf in the docs/ directory).
	// 
  
  int cnt = 0;
  while(cnt < 500000) {
    gpio_write(TRIG, 1);
    delay_us(11);
    gpio_write(TRIG, 0);

    int didWork = timeout_read(ECHO, 0, 1000000);

    int start = timer_get_time();

    didWork &= timeout_read(ECHO, 1, 1000000);

    if(didWork != 1) {
      printk("echo lost...\n");
      continue;
    }

    int duration = timer_get_time() - start;

    int wholepart = duration / 148;

    int decpart = ((duration * 100) / 148) - (wholepart * 100);

    int on_count = ((MAX_DIST - wholepart) * CYCLE_SIZE) / MAX_DIST;
    if(on_count < 0 || on_count > CYCLE_SIZE) on_count = 0;

    printk("GOT - %d.%d at %d%% (on count = %d)\n", wholepart, decpart, (on_count * 100) / CYCLE_SIZE, on_count);
    pwm_compute(duty_cycle, on_count, CYCLE_SIZE);

    delay_ms(100);

    cnt++;
  }


	// troubleshooting:
	//	1. readings can be noisy --- you may need to require multiple
	//	high (or low) readings before you decide to trust the 
	// 	signal.
	//
	// 	2. there are conflicting accounts of what value voltage you
	//	need for Vcc.
	//	
	// 	3. the initial 3 page data sheet you'll find sucks; look for
	// 	a longer one. 

	printk("stopping sonar !\n");
	clean_reboot();
}
