/*
 * blink for arbitrary pins.    
 * Implement:
 *	- gpio_set_output;
 *	- gpio_set_on;
 * 	- gpio_set_off.
 *
 *
 * - try deleting volatile.
 * - change makefile to use -O3
 * - get code to work by calling out to a set32 function to set the address.
 * - initialize a structure with all the fields.
 */

// see broadcomm documents for magic addresses.
#define GPIO_BASE 0x20200000
#include "rpi.h"
volatile unsigned *gpio_fsel0 = (volatile unsigned *)(GPIO_BASE + 0x00);
volatile unsigned *gpio_set0  = (volatile unsigned *)(GPIO_BASE + 0x1C);
volatile unsigned *gpio_clr0  = (volatile unsigned *)(GPIO_BASE + 0x28);

// XXX might need memory barriers.
void gpio_set_output(unsigned pin) {

  unsigned int offset = (pin / 10);
  pin -= (offset * 10);
  if(offset > 5) return;  // Don't pass the maximum offset.

  volatile unsigned* fsel = gpio_fsel0 + offset;

  unsigned int current = get32(fsel);
  unsigned int bits = 0b001 << pin;

  current = (current & ~bits) | bits;

  put32(fsel, current);
}

void gpio_set_on(unsigned pin) {
  unsigned int offset = (pin / 32);
  pin -= offset * 32;
  if(offset > 1) return;

  volatile unsigned* set = gpio_set0 + offset;

  put32(set, 1 << pin);
}
void gpio_set_off(unsigned pin) {
  unsigned int offset = (pin / 32);
  pin -= offset * 32;
  if(offset > 1) return;

  volatile unsigned* clr = gpio_clr0 + offset;

  put32(clr, 1 << pin);
}

// countdown 'ticks' cycles; the asm probably isn't necessary.
void delay(unsigned ticks) {
	while(ticks-- > 0)
		asm("add r1, r1, #0");
}

int notmain ( void ) {
	int led = 20;

  	gpio_set_output(led);
        while(1) {
                gpio_set_on(led);
                delay(1000000);
                gpio_set_off(led);
                delay(1000000);
        }
	return 0;
}
