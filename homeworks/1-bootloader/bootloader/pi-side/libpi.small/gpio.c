/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: gpio.c
 * --------------
 * Provides an interface and constants to interface
 * with the GPIO module of the Raspberry Pi.
 */

#include "rpi.h"
#include "integer.h"
#include "boolean.h"
#include "gpio.h"
#include "timer.h"
#include "mem-barrier.h"
#include "mem-access.h"

/**
 * This global keeps track of GPIO errors -- this allows us
 * to differentiate between errors and what functions otherwise return.
 */
int8 gpio_errno = GPIO_ERR_OK;

// BCM2835 ARM Peripherals §1.2.3 - Physical addresses are written at
// 0x7Ennnnnn in the manual, but are memmapped to 0x20nnnnnn.
#define PERIPHERAL_BASE 0x20000000

// BCM2835AP §6.1 p. 90 - All addresses are offset +0x002000nn
#define GPIO_BASE (PERIPHERAL_BASE + 0x00200000)

// BCM2835AP §6.1 p. 90
/** Registers having to do with pin setup, write, and read. */
static uint32* GPIO_FSEL0 = (void*) (GPIO_BASE + 0x00);
static uint32* GPIO_SET0 = (void*) (GPIO_BASE + 0x1C);
static uint32* GPIO_CLR0 = (void*) (GPIO_BASE + 0x28);
static uint32* GPIO_LEV0 = (void*) (GPIO_BASE + 0x32);

/** Registers having to do with event detection. */
static uint32* GPIO_EDS0 = (void*) (GPIO_BASE + 0x40);
static uint32* GPIO_REN0 = (void*) (GPIO_BASE + 0x4C);
static uint32* GPIO_FEN0 = (void*) (GPIO_BASE + 0x58);
// BCM2835AP §6.1 p. 91
static uint32* GPIO_HEN0 = (void*) (GPIO_BASE + 0x64);
static uint32* GPIO_LEN0 = (void*) (GPIO_BASE + 0x70);
static uint32* GPIO_AREN0 = (void*) (GPIO_BASE + 0x7C);
static uint32* GPIO_AFEN0 = (void*) (GPIO_BASE + 0x88);

/** Registers having to do with pin-pulling. */
static uint32* GPIO_PUD = (void*) (GPIO_BASE + 0x94);
static uint32* GPIO_PUDCLK0 = (void*) (GPIO_BASE + 0x98);

/** Returns false if the requested [pin] is not valid. */
bool gpio_pin_valid(uint8 pin) { return pin >= GPIO_PIN_FIRST && pin <= GPIO_PIN_LAST; }

/**
 * Given a reference address and a reference to a pin number,
 * returns the appropriate register for that pin number,
 * then shifts the referenced pin number to be appropriate
 * for the returned register.
 */
uint32* pin_shift(uint32* gpio_addr, uint8* pin_ref) {
    // FSEL gets special rules. See: BCM2835AP §6.1 pp. 91-94 (fsel mapping: 10 per
    // register, 3 bits per register, except for the last one.
    if (gpio_addr == GPIO_FSEL0) {
        uint32* fsel = GPIO_FSEL0 + (*pin_ref / 10U);
        *pin_ref %= 10;
        return fsel;
    }

    // GPSET, GPCLR, GPLEV, GPEDS, GPREN, GPFEN, GPHEN, GPLEN, GPAREN, GPAGEN, and GPPUDCLK all
    // use the same scheme for mapping pins to registers (32/register, 1 bit per pin).
    // See: BCM2835AP §2.1, pp. 95-100 and p. 101 (skip GPPUD)
    uint32* addr = gpio_addr + (*pin_ref / 32U);
    *pin_ref %= 32U;
    return addr;
}

void gpio_init() {
    gpio_errno = GPIO_ERR_OK;
}

void gpio_reset() {
    for (uint8 pin = GPIO_PIN_FIRST; pin <= GPIO_PIN_LAST; pin++) {
        // BCM2835AP §6.1 p. 91 - Default pin mode is INPUT: "All pins reset to normal GPIO input operation."
        gpio_set_function(pin, GPIO_FUNC_INPUT); // Set all to default.
    }

    gpio_errno = GPIO_ERR_OK;
}

// BCM2835AP §6.1 p. 91
uint8 gpio_set_function(uint8 pin, uint8 function) {
    if (!gpio_pin_valid(pin)) {
        gpio_errno = GPIO_ERR_INVALID_PIN;
        return GPIO_ERROR;
    }

    uint32* fsel = pin_shift(GPIO_FSEL0, &pin);

    data_sync_barrier();
    uint32 current = get32(fsel);

    uint32 bits = ((uint32) function) << (pin * 3);  // Each pin gets 3 bits.
    uint32 full = 0b111U << (pin * 3);

    current = (current & ~full) | bits;  // Clear those bits, then set them according to the given [function]

    put32(fsel, current);
    data_sync_barrier();

    gpio_errno = GPIO_ERR_OK;
    return 0;
}

// BCM2835AP §6.1 p. 91 - Just read instead of write.
uint32 gpio_get_function(uint8 pin) {
    if (!gpio_pin_valid(pin)) {
        gpio_errno = GPIO_ERR_INVALID_PIN;
        return GPIO_ERROR;
    }

    uint32* fsel = pin_shift(GPIO_FSEL0, &pin);

    data_sync_barrier();
    uint32 current = get32(fsel);
    uint32 bits = current >> (pin * 3);

    gpio_errno = GPIO_ERR_OK;
    return bits & 0b111;
}


uint8 gpio_set_input(uint8 pin) { return gpio_set_function(pin, GPIO_FUNC_INPUT); }

uint8 gpio_set_output(uint8 pin) { return gpio_set_function(pin, GPIO_FUNC_OUTPUT); }

// BCM2835AP §6.1 p. 95 - set/clear pins using GPIO_SETn/GPIO_CLRn.
uint8 gpio_write(uint8 pin, uint8 val) {
    if (!gpio_pin_valid(pin)) {
        gpio_errno = GPIO_ERR_INVALID_PIN;
        return GPIO_ERROR;
    }

    data_sync_barrier();

    if (val == 0) {
        put32(pin_shift(GPIO_CLR0, &pin), 1U << pin);
    } else {
        put32(pin_shift(GPIO_SET0, &pin), 1U << (pin % 32)); // pin % 32 ensures pin is adjusted even with pin_shift, just in case of weird cache stuff.
    }

    data_sync_barrier();

    gpio_errno = GPIO_ERR_OK;
    return 0;
}

// BCM2835AP §6.1 p. 96 - GPLEVn will retrieve the status of the given GPIO pin.
uint8 gpio_read(uint8 pin) {
    if (!gpio_pin_valid(pin)) {
        gpio_errno = GPIO_ERR_INVALID_PIN;
        return GPIO_ERROR;
    }

    data_sync_barrier();
    uint32* lev = pin_shift(GPIO_LEV0, &pin);

    gpio_errno = GPIO_ERR_OK;
    return getBit(lev, pin);
}

// BCM2835AP §6.1 pp. 97-100 - set a 1 to detect the event, set a 0 to do the reverse.
uint8 gpio_set_detect_event(uint8 pin, uint8 event, bool enabled) {
    if(event < MIN_EVENT || event > MAX_EVENT) {
        gpio_errno = GPIO_ERR_INVALID_EVENT;
        return GPIO_ERROR;
    }

    // events are mapped 0-5 in this order, so we can succinctly pull the event register like this.
    uint32* reg0 = (uint32*[]){GPIO_REN0, GPIO_FEN0, GPIO_HEN0, GPIO_LEN0, GPIO_AREN0, GPIO_AFEN0}[event];
    uint32* reg = pin_shift(reg0, &pin);

    data_sync_barrier();
    setBit(reg, pin, (uint8) enabled);
    data_sync_barrier();

    gpio_errno = GPIO_ERR_OK;
    return 0;
}

// BCM2835AP §6.1 p. 96 - GPEDSn - Registers are 32bit and each pin as 1 bit.
bool gpio_check_event(uint8 pin) {
    if (!gpio_pin_valid(pin)) {
        gpio_errno = GPIO_ERR_INVALID_PIN;
        return GPIO_ERROR;
    }

    data_sync_barrier();
    uint32* reg = pin_shift(GPIO_EDS0, &pin);

    gpio_errno = GPIO_ERR_OK;
    return getBit(reg, pin);
}

// BCM2835AP §6.1 p. 96 - GPEDSn - Events are cleared by writing a 1.
bool gpio_check_and_clear_event(uint8 pin) {
    if (!gpio_pin_valid(pin)) {
        gpio_errno = GPIO_ERR_INVALID_PIN;
        return GPIO_ERROR;
    }

    data_sync_barrier();
    uint32* reg = pin_shift(GPIO_EDS0, &pin);
    uint8 hasEvent = getBit(reg, pin);

    uint32 mask = (1U << pin);
    put32(GPIO_EDS0, mask); // Writing a 1 clears the bit (BCM2835AP p. 96)
    data_sync_barrier();

    gpio_errno = GPIO_ERR_OK;
    return hasEvent;
}

// Process documented on BCM2835AP §6.1 p. 101.
uint8 pin_pull(uint8 pin, uint32 pud) {
    if (!gpio_pin_valid(pin)) {
        gpio_errno = GPIO_ERR_INVALID_PIN;
        return GPIO_ERROR;
    }

    put32(GPIO_PUD, pud);      // 1. Write control signal appropriate for pull-control.
    delay(150);                // 2. Wait 150 cycles

    uint32* clk = pin_shift(GPIO_PUDCLK0, &pin);
    put32(clk, 1U << pin);     // 3. Write to GPPUDCLK to clock the GPIO pads
    delay(150);                // 4. Wait 150 cycles

    put32(GPIO_PUD, 0);        // 5. Write to GPPUD to remove the control signal

    put32(clk, 0);             // 6. Write to GPPUDCLK to remove the clock.
    data_sync_barrier();

    gpio_errno = GPIO_ERR_OK;
    return 0;
}
