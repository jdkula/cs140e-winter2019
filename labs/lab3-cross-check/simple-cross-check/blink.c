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
int gpio_set_output(unsigned pin) {
  if(pin > 31) return -1;

  unsigned int offset = (pin / 10);
  pin -= (offset * 10);
  if(offset > 5) return -1;  // Don't pass the maximum offset.

  volatile unsigned* fsel = gpio_fsel0 + offset;

  unsigned int current = get32(fsel);
  unsigned int bits = 0b001 << (pin * 3);

  current = (current & ~(0b111 << (pin * 3))) | bits;

  put32(fsel, current);
  return 0;
}

int gpio_set_on(unsigned pin) {
  if(pin > 31) return -1;

  unsigned int offset = (pin / 32);
  pin -= offset * 32;
  if(offset > 1) return -1;

  volatile unsigned* set = gpio_set0 + offset;

  put32(set, 1 << pin);
  return 0;
}

int gpio_set_off(unsigned pin) {
  if(pin > 31) return -1;

  unsigned int offset = (pin / 32);
  pin -= offset * 32;
  if(offset > 1) return -1;

  volatile unsigned* clr = gpio_clr0 + offset;

  put32(clr, 1 << pin);
  return 0;
}

// countdown 'ticks' cycles; the asm probably isn't necessary.
//void delay(unsigned ticks) {
//	while(ticks-- > 0)
//		asm("add r1, r1, #0");
//}

void notmain ( void ) {
	int led = 20;

  	gpio_set_output(led);
        for(int i = 0; i < 100; i++) {
                gpio_set_on(led);
                delay(1000000);
                gpio_set_off(led);
                delay(1000000);
        }
}
