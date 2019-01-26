#include <rpi.h>
#include <mem-access.h>
#include <mem-barrier.h>
#include "gpio.h"
#include "timer.h"

#define GPIO_BASE 0x20200000
#define PULLDOWN  1
#define PULLUP    2

static unsigned* GPIO_FSEL0 = (void*) (GPIO_BASE + 0x00);
static unsigned* GPIO_SET0 = (void*) (GPIO_BASE + 0x1C);
static unsigned* GPIO_CLR0 = (void*) (GPIO_BASE + 0x28);
static unsigned* GPIO_LEV0 = (void*) (GPIO_BASE + 0x32);

static unsigned* GPIO_EDS0 = (void*) (GPIO_BASE + 0x40);
static unsigned* GPIO_REN0 = (void*) (GPIO_BASE + 0x4C);
static unsigned* GPIO_FEN0 = (void*) (GPIO_BASE + 0x58);

static unsigned* GPIO_PUD = (void*) (GPIO_BASE + 0x94);
static unsigned* GPIO_UDCLK0 = (void*) (GPIO_BASE + 0x98);

unsigned char _gpio_pin_valid(unsigned char pin) { return pin >= GPIO_PIN_FIRST && pin <= GPIO_PIN_LAST; }

void gpio_init() { /* No-op */ }

unsigned* gpio_pin_to_function_register(unsigned char pin) {
    unsigned offset = (pin / 10U);
    if (offset > 5) return 0;  // Don't pass the maximum offset.

    unsigned* fsel = GPIO_FSEL0 + offset;

    return fsel;
}

void gpio_set_function(unsigned char pin, unsigned char function) {
    //assert(_gpio_pin_valid(pin));
    unsigned* fsel = gpio_pin_to_function_register(pin);

    pin = (unsigned char) (pin % 10U);

    //dsb(); // data sync barrier
    unsigned current = get32(fsel);
    unsigned bits = ((unsigned) function) << (pin * 3);
    unsigned full = 0b111U << (pin * 3);

    current = (current & ~full) | bits;

    put32(fsel, current);
    //dsb(); // data sync barrier
}

unsigned gpio_get_function(unsigned char pin) {
    //assert(_gpio_pin_valid(pin));
    unsigned* fsel = gpio_pin_to_function_register(pin);

    dsb(); // data sync barrier
    unsigned current = get32(fsel);
    unsigned bits = current >> (pin * 3);

    return bits & 0b111;
}


void gpio_set_input(unsigned char pin) { gpio_set_function(pin, GPIO_FUNC_INPUT); }

void gpio_set_output(unsigned char pin) { gpio_set_function(pin, GPIO_FUNC_OUTPUT); }

void gpio_write(unsigned char pin, unsigned char val) {
    //assert(_gpio_pin_valid(pin));
    if (val == 0) {
        put32(GPIO_CLR0, 1U << pin);
    } else {
        put32(GPIO_SET0, 1U << pin);
    }
}

unsigned char gpio_read(unsigned char pin) {
    //assert(_gpio_pin_valid(pin));

    dsb(); // data sync barrier
    unsigned value = get32(GPIO_LEV0);

    return (unsigned char) ((value >> pin) & 0b1);
}

void gpio_detect_falling_edge(unsigned char pin) {
    //assert(_gpio_pin_valid(pin));
    dsb(); // data sync barrier
    unsigned int val = get32(GPIO_FEN0);
    val |= 1 << pin;
    put32(GPIO_FEN0, val);
    dsb(); // data sync barrier
}

void gpio_detect_rising_edge(unsigned char pin) {
    //assert(_gpio_pin_valid(pin));
    dsb(); // data sync barrier
    unsigned int val = get32(GPIO_REN0);
    val |= 1 << pin;
    put32(GPIO_FEN0, val);
    dsb(); // data sync barrier
}

unsigned gpio_check_event(unsigned char pin) {
    //assert(_gpio_pin_valid(pin));
    dsb(); // data sync barrier
    unsigned int val = get32(GPIO_EDS0);
    return (val & (1 << pin));
}

unsigned gpio_check_and_clear_event(unsigned char pin) {
    //assert(_gpio_pin_valid(pin));
    dsb(); // data sync barrier
    unsigned int val = get32(GPIO_EDS0);
    unsigned int mask = (1U << pin);
    put32(GPIO_EDS0, mask); // Writing a 1 clears the bit
    dsb(); // data sync barrier
    return (val & mask);
}

// These functions are a little complex, they require some
// timing and a busy loop for ~150 cycles. So do them later.
unsigned gpio_pin_to_udclock_offset(unsigned char pin) {
    //assert(_gpio_pin_valid(pin));
    return pin & 0x1fU;
}

void pin_pull(unsigned char pin, unsigned int pud) {
    //assert(_gpio_pin_valid(pin));
    put32(GPIO_PUD, pud & 3);
    // timer_wait_for(5);

    unsigned int offset = gpio_pin_to_udclock_offset(pin);
    put32(GPIO_UDCLK0, 1U << (offset));
    // timer_wait_for(5);

    put32(GPIO_UDCLK0, 0);
    // timer_wait_for(5);

    put32(GPIO_UDCLK0, 0);
    // timer_wait_for(5);
    dsb(); // data sync barrier
}

void gpio_set_pullup(unsigned char pin) {
    //assert(_gpio_pin_valid(pin));
    pin_pull(pin, PULLUP);
}

void gpio_set_pulldown(unsigned char pin) {
    //assert(_gpio_pin_valid(pin));
    pin_pull(pin, PULLDOWN);
}
