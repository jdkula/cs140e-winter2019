#include <stdint.h>

#include <stdbool.h>
#include <assert.h>
#include "gpio.h"
#include "mem-barrier.h"
#include "timer.h"

#define GPIO_BASE 0x20200000
#define PULLDOWN  1
#define PULLUP    2

static volatile uint32_t* GPIO_FSEL0  = (volatile uint32_t*)(GPIO_BASE + 0x00);
static volatile uint32_t* GPIO_SET0   = (volatile uint32_t*)(GPIO_BASE + 0x1C);
static volatile uint32_t* GPIO_CLR0   = (volatile uint32_t*)(GPIO_BASE + 0x28);
static volatile uint32_t* GPIO_LEV0   = (volatile uint32_t*)(GPIO_BASE + 0x32);

static volatile uint32_t* GPIO_EDS0   = (volatile uint32_t*)(GPIO_BASE + 0x40);
static volatile uint32_t* GPIO_REN0   = (volatile uint32_t*)(GPIO_BASE + 0x4C);
static volatile uint32_t* GPIO_FEN0   = (volatile uint32_t*)(GPIO_BASE + 0x58);

static volatile uint32_t* GPIO_PUD    = (volatile uint32_t*)(GPIO_BASE + 0x94);
static volatile uint32_t* GPIO_UDCLK0 = (volatile uint32_t*)(GPIO_BASE + 0x98);

uint32_t get32(volatile uint32_t* ptr) { return *ptr; }
void put32(volatile uint32_t* ptr, uint32_t data) { *ptr = data; }

bool _gpio_pin_valid(uint8_t pin) { return pin >= GPIO_PIN_FIRST && pin <= GPIO_PIN_LAST; }

void gpio_init() { /* No-op */ }

volatile uint32_t* _gpio_pin_to_function_register(uint8_t pin) {
  uint32_t offset = (pin / 10);
  pin -= (offset * 10);
  if(offset > 5) return 0;  // Don't pass the maximum offset.

  volatile uint32_t* fsel = GPIO_FSEL0 + offset;

  return fsel;
}

void gpio_set_function(uint8_t pin, uint8_t function) {
  assert(_gpio_pin_valid(pin));
  volatile uint32_t* fsel = _gpio_pin_to_function_register(pin);

  dsb(); // data sync barrier
  uint32_t current = get32(fsel);
  uint32_t bits = function << (pin * 3);

  current = (current & ~bits) | bits;

  put32(fsel, current);
  dsb(); // data sync barrier
}

uint32_t gpio_get_function(uint8_t pin) {
  assert(_gpio_pin_valid(pin));
  volatile uint32_t* fsel = _gpio_pin_to_function_register(pin);

  dsb(); // data sync barrier
  uint32_t current = get32(fsel);
  uint32_t bits = current >> (pin * 3);

  return bits & 0b111;
}


void gpio_set_input(uint8_t pin) { gpio_set_function(pin, GPIO_FUNC_INPUT); }
void gpio_set_output(uint8_t pin) { gpio_set_function(pin, GPIO_FUNC_OUTPUT); }

void gpio_write(uint8_t pin, uint8_t val) {
  assert(_gpio_pin_valid(pin));
  if(val == 0) {
    put32(GPIO_CLR0, 1 << pin);
  } else {
    put32(GPIO_SET0, 1 << pin);
  }
}

uint8_t gpio_read(uint8_t pin) {
  assert(_gpio_pin_valid(pin));

  dsb(); // data sync barrier
  uint32_t value = get32(GPIO_LEV0);

  return (value >> pin) & 0b1;
}

void gpio_detect_falling_edge(unsigned int pin) {
  dsb(); // data sync barrier
  unsigned int val = get32(GPIO_FEN0);
  val |= 1 << pin;
  put32(GPIO_FEN0, val);
  dsb(); // data sync barrier
}

void gpio_detect_rising_edge(unsigned int pin) {
  dsb(); // data sync barrier
  unsigned int val = get32(GPIO_REN0);
  val |= 1 << pin;
  put32(GPIO_FEN0, val);
  dsb(); // data sync barrier
}

unsigned int gpio_check_event(unsigned int pin) {
  dsb(); // data sync barrier
  unsigned int val = get32(GPIO_EDS0);
  return (val & (1 << pin));
}

unsigned int gpio_check_and_clear_event(unsigned int pin) {
  dsb(); // data sync barrier
  unsigned int val = get32(GPIO_EDS0);
  unsigned int mask = (1 << pin);
  put32(GPIO_EDS0, mask); // Writing a 1 clears the bit
  dsb(); // data sync barrier
  return (val & mask);
}

// These functions are a little complex, they require some
// timing and a busy loop for ~150 cycles. So do them later.
unsigned int gpio_pin_to_udclock_offset(unsigned int pin) {
  return pin & 0x1f;
}

void pin_pull(unsigned int pin, unsigned int pud) {
  put32(GPIO_PUD, pud & 3);
  timer_wait_for(5);
  
  unsigned int offset = gpio_pin_to_udclock_offset(pin);
  put32(GPIO_UDCLK0, 1 << (offset));
  timer_wait_for(5);
  
  put32(GPIO_UDCLK0, 0);
  timer_wait_for(5);
  
  put32(GPIO_UDCLK0, 0);
  timer_wait_for(5);
  dsb(); // data sync barrier
}

void gpio_set_pullup(uint8_t pin) {
  pin_pull(pin, PULLUP);
}

void gpio_set_pulldown(uint8_t pin) {
  pin_pull(pin, PULLDOWN);
}
