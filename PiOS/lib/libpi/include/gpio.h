/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: gpio.h
 * --------------
 * Provides an interface and constants to interface
 * with the GPIO module of the Raspberry Pi.
 */

#ifndef PIOS_GPIO_H
#define PIOS_GPIO_H

#include <integer.h>
#include <boolean.h>

/** Functions return GPIO_ERROR and set gpio_errno when there's an issue. */
#define GPIO_ERROR 127
enum {
    GPIO_ERR_OK,                // Indicates the absence of an error.
    GPIO_ERR_INVALID_PIN,       // Indicates that an invalid pin was used.
    GPIO_ERR_INVALID_EVENT,     // Indicates an invalid event was used.
    GPIO_ERR_INVALID_FUNCTION,  // Indicates an invalid function was used.
};
extern int8 gpio_errno;

/**
 * These enumerated values establish symbolic names for each of the GPIO pins.
 * Some pins have multiple values, if they have multiple functions.
 */
enum {
    GPIO_PIN_FIRST = 0U,
    GPIO_PIN0 = 0U,
    GPIO_PIN1 = 1U,
    GPIO_PIN2 = 2U,
    GPIO_PIN3 = 3U,
    GPIO_PIN4 = 4U,
    GPIO_PIN5 = 5U,
    GPIO_PIN6 = 6U,
    GPIO_PIN7 = 7U,
    GPIO_PIN8 = 8U,
    GPIO_PIN9 = 9U,
    GPIO_PIN10 = 10U,
    GPIO_PIN11 = 11U,
    GPIO_PIN12 = 12U,
    GPIO_PIN13 = 13U,
    GPIO_PIN14 = 14U,
    GPIO_PIN15 = 15U,
    GPIO_PIN16 = 16U,
    GPIO_PIN17 = 17U,
    GPIO_PIN18 = 18U,
    GPIO_PIN19 = 19U,
    GPIO_PIN20 = 20U,
    GPIO_PIN21 = 21U,
    GPIO_PIN22 = 22U,
    GPIO_PIN23 = 23U,
    GPIO_PIN24 = 24U,
    GPIO_PIN25 = 25U,
    GPIO_PIN26 = 26U,
    GPIO_PIN27 = 27U,
    GPIO_PIN28 = 28U,
    GPIO_PIN29 = 29U,
    GPIO_PIN30 = 30U,
    GPIO_PIN31 = 31U,
    GPIO_PIN32 = 32U,
    GPIO_PIN33 = 33U,
    GPIO_PIN34 = 34U,
    GPIO_PIN35 = 35U,
    GPIO_PIN36 = 36U,
    GPIO_PIN37 = 37U,
    GPIO_PIN38 = 38U,
    GPIO_PIN39 = 39U,
    GPIO_PIN40 = 40U,
    GPIO_PIN41 = 41U,
    GPIO_PIN42 = 42U,
    GPIO_PIN43 = 43U,
    GPIO_PIN44 = 44U,
    GPIO_PIN45 = 45U,
    GPIO_PIN46 = 46U,
    GPIO_PIN47 = 47U,
    GPIO_PIN48 = 48U,
    GPIO_PIN49 = 49U,
    GPIO_PIN50 = 50U,
    GPIO_PIN51 = 51U,
    GPIO_PIN52 = 52U,
    GPIO_PIN53 = 53U,
    GPIO_PIN_LAST =  53U, // BCM2835 p. 94 -- last function option is FSEL53.

    GPIO_TX = GPIO_PIN14, // As indicated on the Raspberry Pi
    GPIO_RX = GPIO_PIN15,

    // Source: https://raspberrypi.stackexchange.com/questions/67431/cant-turn-on-act-led-on-baremetal-pi3
    GPIO_ACT = GPIO_PIN47,
    GPIO_PWR = GPIO_PIN35
};

/**
 * These enumerated values establish symbolic names for each of the
 * available GPIO pin functions. Each pin function corresponds to
 * a particular "mode" of operation. For example, setting a pin's
 * function to GPIO_FUNC_INPUT configures the pin to be used as an input.
 * Some functions have multiple names, if that makes semantic sense.
 */
enum {  // BCM2835 pp. 92-94 - table contains bit-to-function mappings.
    GPIO_FUNC_INPUT = 0b000,
    GPIO_FUNC_OUTPUT = 0b001,
    GPIO_FUNC_ALT0 = 0b100,
    GPIO_FUNC_ALT1 = 0b101,
    GPIO_FUNC_ALT2 = 0b110,
    GPIO_FUNC_ALT3 = 0b111,
    GPIO_FUNC_ALT4 = 0b011,
    GPIO_FUNC_ALT5 = 0b010,

    GPIO_FUNC_UART = GPIO_FUNC_ALT5 // BCM2835AP §6.2 p. 102 - TXD1/RXD1 for pins 14 and 15.
};

/** This enum gives symbolic names for a GPIO pin on (HIGH) vs off (LOW). */
enum {
    HIGH = 1,
    LOW = 0
};

/**
 * Initialize the GPIO code module. Since GPIO requires no initialization, this does nothing except set gpio_errno.
 */
void gpio_init(void);

/**
 * Reset GPIO. Sets all GPIO pins to INPUT.
 */
void gpio_reset(void);

/**
 * Set a GPIO function for GPIO pin number `pin`. Settings for other pins
 * should be unchanged.
 *
 * @param pin      the GPIO pin number to configure
 * @param function the GPIO function to set for the pin
 * @return 0 on success, GPIO_ERROR on error.
 *
 * If `pin` or `function` is invalid, does nothing.
 */
uint8 gpio_set_function(uint8 pin, uint8 function);

/**
 * Get the GPIO function for GPIO pin number `pin`.
 *
 * @param pin the GPIO pin number to read the function of
 * @return    the current GPIO function of the specified pin
 *
 * If `pin` is invalid, returns GPIO_INVALID_REQUEST.
 */
uint32 gpio_get_function(uint8 pin);

/**
 * Convenience functions for setting a pin to GPIO_FUNC_INPUT or
 * GPIO_FUNC_OUTPUT. The implementation calls `gpio_set_function`.
 *
 * @param pin the GPIO pin number to set the function of
 * @return 0 on success, GPIO_ERROR otherwise.
 */
uint8 gpio_set_input(uint8 pin);

uint8 gpio_set_output(uint8 pin);

/**
 * Set GPIO pin number `pin` to high (1) or low (0). This
 * function assumes the pin is already in output mode.
 * Settings for other pins should be unchanged.
 *
 * @param pin   the GPIO pin number to set or clear
 * @param val   1 to set pin to high, 0 to clear pin
 * @return 0 on success, GPIO_ERROR otherwise.
 *
 * If `pin` is invalid, does nothing.
 */
uint8 gpio_write(uint8 pin, uint8 val);

/**
 * Get current level (1 for high, 0 for low) for GPIO pin number `pin`.
 *
 * @param pin the GPIO pin number to read the value of
 * @return    the value of the specified pin
 *
 * If `pin` is invalid, returns GPIO_INVALID_REQUEST.
 */
uint8 gpio_read(uint8 pin);

/**
 * Sets event detection (of the given [event]) on the
 * given [pin] to be either disabled or [enabled].
 */
uint8 gpio_set_detect_event(uint8 pin, uint8 event, bool enabled);

enum {
    GPIO_EVENT_RISING_EDGE = 0,
    GPIO_EVENT_FALLING_EDGE = 1,
    GPIO_EVENT_HIGH = 2,
    GPIO_EVENT_LOW = 3,
    GPIO_EVENT_RISING_EDGE_ASYNC = 4,
    GPIO_EVENT_FALLING_EDGE_ASYNC = 5,

    MIN_EVENT = GPIO_EVENT_RISING_EDGE,
    MAX_EVENT = GPIO_EVENT_FALLING_EDGE_ASYNC
};

/** Returns true if an event has occurred on the given [pin]. */
bool gpio_check_event(uint8 pin);

/** Returns true if an event has occurred on the given [pin]. Also clears the event. */
bool gpio_check_and_clear_event(uint8 pin);

/** Constants for gpio_set_pud -- symbolic representations of the pull-state of a pin. */
enum {
    GPIO_PUD_DISABLE = 0,
    GPIO_PUD_PULLDOWN = 0b1,
    GPIO_PUD_PULLUP = 0b10,
};

/** Sets the given [pin] to the pull-state given by [pud]. */
uint8 pin_pull(uint8 pin, uint32 pud);

/** Convenience methods that pull a pin up/down. */
uint8 gpio_set_pulldown(uint8 pin);
uint8 gpio_set_pullup(uint8 pin);


#endif // PIOS_GPIO_H
