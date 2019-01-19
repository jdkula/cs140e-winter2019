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
#include "rpi.h"

unsigned int gpio_pin_valid(unsigned int pin) {
  return (pin >= GPIO_PIN_FIRST && pin <= GPIO_PIN_LAST);
}

unsigned int gpio_function_valid(unsigned int f) {
  return (f >= GPIO_FUNC_INPUT && f <= GPIO_FUNC_ALT5);
}
 
unsigned int gpio_is_input(unsigned int pin) {
  return gpio_get_function(pin) == GPIO_FUNC_INPUT;
}

unsigned int gpio_is_output(unsigned int pin) {
  return gpio_get_function(pin) == GPIO_FUNC_OUTPUT;
}

unsigned int gpio_get_function(unsigned int pin) {
  if (!gpio_pin_valid(pin)) {return GPIO_FUNC_INVALID;}
  system_memory_read_barrier();
  
  unsigned int reg = gpio_pin_to_function_register(pin);
  unsigned int val = GET32(reg);
  val = val >> gpio_pin_to_function_offset(pin);
  val &= 0x7;
  return val;
}

unsigned int gpio_set_input(unsigned int pin) {
  return gpio_set_function(pin, GPIO_FUNC_INPUT);
}

unsigned int gpio_set_output(unsigned int pin) {
  return gpio_set_function(pin, GPIO_FUNC_OUTPUT);
}

unsigned int gpio_set_function(unsigned int pin, unsigned int function) {
  if (!gpio_pin_valid(pin)) {return GPIO_FUNC_INVALID;}
  if (!gpio_function_valid(function)) {return GPIO_FUNC_INVALID;}

  system_memory_read_barrier();
  unsigned int reg = gpio_pin_to_function_register(pin);
  unsigned int offset = gpio_pin_to_function_offset(pin);
  unsigned int val = GET32(reg);

  // Remove the original configuration bits
  unsigned int mask = ~(0x7 << offset);
  val = val & mask;

  // Add the new configuration bits
  val |= function << offset;

  // Set the register
  PUT32(reg, val);
  system_memory_write_barrier();
  return function;
}
                      
unsigned int gpio_pin_read(unsigned int pin) {
  unsigned int reg = gpio_pin_to_input_register(pin);
  system_memory_read_barrier();
  unsigned int val = GET32(reg);
  val = val >> gpio_pin_to_input_offset(pin);
  val = val & 1;
  return val;
}

void gpio_pin_write(unsigned int pin, unsigned int boolean) {
  unsigned int reg;
  unsigned int offset;
  
  if (boolean) {
    reg = gpio_pin_to_set_register(pin);
    offset = gpio_pin_to_set_offset(pin);
  } else {
    reg = gpio_pin_to_clear_register(pin);
    offset = gpio_pin_to_clear_offset(pin);
  }

  unsigned int val = GET32(reg);
  val |= 1 << offset;
  PUT32(reg, val);
  system_memory_write_barrier();
}

void gpio_detect_falling_edge(unsigned int pin) {
  unsigned int reg;
  unsigned int offset;

  reg = gpio_pin_to_falling_register(pin);
  offset = gpio_pin_to_falling_offset(pin);

  system_memory_read_barrier();
  unsigned int val = GET32(reg);
  val |= 1 << offset;
  PUT32(reg, val);
  system_memory_write_barrier();
}

void gpio_detect_rising_edge(unsigned int pin) {
  unsigned int reg;
  unsigned int offset;

  reg = gpio_pin_to_rising_register(pin);
  offset = gpio_pin_to_rising_offset(pin);
  system_memory_read_barrier();
  unsigned int val = GET32(reg);
  val |= 1 << offset;
  PUT32(reg, val);
  system_memory_write_barrier();
}

unsigned int gpio_check_event(unsigned int pin) {
  unsigned int reg;
  unsigned int offset;

  reg = gpio_pin_to_event_register(pin);
  offset = gpio_pin_to_event_offset(pin);
  system_memory_read_barrier();
  unsigned int val = GET32(reg);
  return (val & (1 << offset));
}

unsigned int gpio_check_and_clear_event(unsigned int pin) {
  unsigned int reg;
  unsigned int offset;

  reg = gpio_pin_to_event_register(pin);
  offset = gpio_pin_to_event_offset(pin);
  system_memory_read_barrier();
  unsigned int val = GET32(reg);
  unsigned int mask = (1 << offset);
  PUT32(reg, mask); // Writing a 1 clears the bit
  system_memory_write_barrier();
  return (val & mask);
}

// These functions are a little complex, they require some
// timing and a busy loop for ~150 cycles. So do them later.

unsigned int gpio_pin_to_udclock_register(unsigned int pin) {
  if (pin <= 31) {
    return GPPUDCLK0;
  } else {
    return GPPUDCLK1;
  }
}
unsigned int gpio_pin_to_udclock_offset(unsigned int pin) {
  return pin & 0x1f;
}

void pin_pull(unsigned int pin, unsigned int pud) {
  PUT32(GPPUD, pud & 3 );
  timer_wait_for(5);
  
  unsigned int reg = gpio_pin_to_udclock_register(pin);
  unsigned int offset = gpio_pin_to_udclock_offset(pin);
  PUT32(reg, 1 << (offset));
  timer_wait_for(5);
  
  PUT32(GPPUD, 0);
  timer_wait_for(5);
  
  PUT32(GPPUDCLK0, 0);
  timer_wait_for(5);
  system_memory_write_barrier();
}

void gpio_set_pullup(unsigned int pin) {
  pin_pull(pin, PULLUP);
}

void gpio_set_pulldown(unsigned int pin) {
  pin_pull(pin, PULLDOWN);
}
/*
 * Copyright (c) 2014 Stanford University.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the Stanford University nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL STANFORD
 * UNIVERSITY OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Date: 9/26/2014
 */

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
