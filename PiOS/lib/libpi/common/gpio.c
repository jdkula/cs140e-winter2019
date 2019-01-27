#include <rpi.h>
#include <mem-access.h>
#include <mem-barrier.h>
#include <integer.h>
#include "gpio.h"
#include "timer.h"

#define GPIO_BASE 0x20200000
#define PULLDOWN  1
#define PULLUP    2

static uint32* GPIO_FSEL0 = (void*) (GPIO_BASE + 0x00);
static uint32* GPIO_SET0 = (void*) (GPIO_BASE + 0x1C);
static uint32* GPIO_CLR0 = (void*) (GPIO_BASE + 0x28);
static uint32* GPIO_LEV0 = (void*) (GPIO_BASE + 0x32);

static uint32* GPIO_EDS0 = (void*) (GPIO_BASE + 0x40);
static uint32* GPIO_REN0 = (void*) (GPIO_BASE + 0x4C);
static uint32* GPIO_FEN0 = (void*) (GPIO_BASE + 0x58);

static uint32* GPIO_PUD = (void*) (GPIO_BASE + 0x94);
static uint32* GPIO_UDCLK0 = (void*) (GPIO_BASE + 0x98);

uint8 _gpio_pin_valid(uint8 pin) { return pin >= GPIO_PIN_FIRST && pin <= GPIO_PIN_LAST; }

void gpio_init() {
    for(uint8 pin = GPIO_PIN_FIRST; pin <= GPIO_PIN_LAST; pin++) {
        gpio_set_function(pin, GPIO_FUNC_INPUT); // Set all to default.
    }
}

uint32* pin_shift(uint32* gpio_addr, uint8* pin_ref) {
    if(gpio_addr == GPIO_FSEL0) {
        uint32* fsel = GPIO_FSEL0 + (*pin_ref / 10U);
        *pin_ref %= 10;
        return fsel;
    }
    uint32* addr = gpio_addr + (*pin_ref / 32U);
    *pin_ref = (uint8) (*pin_ref % 32U);
    return addr;
}


void gpio_set_function(uint8 pin, uint8 function) {
    //assert(_gpio_pin_valid(pin));
    uint32* fsel = pin_shift(GPIO_FSEL0, &pin);

    pin = (uint8) (pin % 10U);

    //data_sync_barrier(); // data sync barrier
    uint32 current = get32(fsel);
    uint32 bits = ((uint32) function) << (pin * 3);
    uint32 full = 0b111U << (pin * 3);

    current = (current & ~full) | bits;

    put32(fsel, current);
    //data_sync_barrier(); // data sync barrier
}

uint32 gpio_get_function(uint8 pin) {
    //assert(_gpio_pin_valid(pin));
    uint32* fsel = pin_shift(GPIO_FSEL0, &pin);

    data_sync_barrier(); // data sync barrier
    uint32 current = get32(fsel);
    uint32 bits = current >> (pin * 3);

    return bits & 0b111;
}


void gpio_set_input(uint8 pin) { gpio_set_function(pin, GPIO_FUNC_INPUT); }

void gpio_set_output(uint8 pin) { gpio_set_function(pin, GPIO_FUNC_OUTPUT); }

void gpio_write(uint8 pin, uint8 val) {
    //assert(_gpio_pin_valid(pin));
    if (val == 0) {
        put32(pin_shift(GPIO_CLR0, &pin), 1U << pin);
    } else {
        put32(pin_shift(GPIO_SET0, &pin), 1U << (pin % 32));
    }
}

uint8 gpio_read(uint8 pin) {
    //assert(_gpio_pin_valid(pin));

    data_sync_barrier(); // data sync barrier
    uint32 value = get32(GPIO_LEV0);

    return (uint8) ((value >> pin) & 0b1);
}

void gpio_detect_falling_edge(uint8 pin) {
    //assert(_gpio_pin_valid(pin));
    data_sync_barrier(); // data sync barrier
    uint32 val = get32(GPIO_FEN0);
    val |= 1 << pin;
    put32(GPIO_FEN0, val);
    data_sync_barrier(); // data sync barrier
}

void gpio_detect_rising_edge(uint8 pin) {
    //assert(_gpio_pin_valid(pin));
    data_sync_barrier(); // data sync barrier
    uint32 val = get32(GPIO_REN0);
    val |= 1 << pin;
    put32(GPIO_FEN0, val);
    data_sync_barrier(); // data sync barrier
}

uint32 gpio_check_event(uint8 pin) {
    //assert(_gpio_pin_valid(pin));
    data_sync_barrier(); // data sync barrier
    uint32 val = get32(GPIO_EDS0);
    return (val & (1 << pin));
}

uint32 gpio_check_and_clear_event(uint8 pin) {
    //assert(_gpio_pin_valid(pin));
    data_sync_barrier(); // data sync barrier
    uint32 val = get32(GPIO_EDS0);
    uint32 mask = (1U << pin);
    put32(GPIO_EDS0, mask); // Writing a 1 clears the bit
    data_sync_barrier(); // data sync barrier
    return (val & mask);
}

// These functions are a little complex, they require some
// timing and a busy loop for ~150 cycles. So do them later.
uint32 gpio_pin_to_udclock_offset(uint8 pin) {
    //assert(_gpio_pin_valid(pin));
    return pin & 0x1fU;
}

void pin_pull(uint8 pin, uint32 pud) {
    //assert(_gpio_pin_valid(pin));
    put32(GPIO_PUD, pud & 3);
    // timer_wait_for(5);

    uint32 offset = gpio_pin_to_udclock_offset(pin);
    put32(GPIO_UDCLK0, 1U << (offset));
    // timer_wait_for(5);

    put32(GPIO_UDCLK0, 0);
    // timer_wait_for(5);

    put32(GPIO_UDCLK0, 0);
    // timer_wait_for(5);
    data_sync_barrier(); // data sync barrier
}

void gpio_set_pullup(uint8 pin) {
    //assert(_gpio_pin_valid(pin));
    pin_pull(pin, PULLUP);
}

void gpio_set_pulldown(uint8 pin) {
    //assert(_gpio_pin_valid(pin));
    pin_pull(pin, PULLDOWN);
}
