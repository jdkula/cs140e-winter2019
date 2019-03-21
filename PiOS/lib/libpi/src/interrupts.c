/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: interrupts.c
 * --------------
 * Provides methods to initialize, enable, and disable
 * interrupts of various types. Also holds arrays of function
 * pointers that are invoked upon interrupts.
 */

#include <gpio.h>
#include <mem-access.h>
#include "rpi.h"
#include "interrupts.h"
#include "demand.h"

#define MAX_TIMER_INTS 128

// holds function pointers to an interrupt handler per pin.
gpio_interrupt_t gpio_interrupts[GPIO_PIN_LAST] = {0};

// holds an array of function pointers that are invoked when the timer fires.
timer_interrupt_t timer_interrupts[MAX_TIMER_INTS] = {0};
uint8_t timer_ints = 0; // holds the number of set timer interrupts.

// interrupt macros are defined in interrupts.h and should be OR'd together here.
void initialize_interrupts(uint32_t basic, uint32_t reg1, uint32_t reg2) {
    install_interrupt_handlers();

    disable_interrupts(~basic, ~reg1, ~reg2);
    enable_interrupts(basic, reg1, reg2);
    system_enable_interrupts();
}

void disable_interrupts(uint32_t basic, uint32_t reg1, uint32_t reg2) {
    volatile rpi_irq_controller_t* irq = get_irq_controller();

    put32((void*) &irq->disable_basic_irqs, basic);
    put32((void*) &irq->disable_irqs_1, reg1);
    put32((void*) &irq->disable_irqs_2, reg2);
}

void enable_interrupts(uint32_t basic, uint32_t reg1, uint32_t reg2) {
    volatile rpi_irq_controller_t* irq = get_irq_controller();

    put32((void*) &irq->enable_basic_irqs, basic);
    put32((void*) &irq->enable_irqs_1, reg1);
    put32((void*) &irq->enable_irqs_2, reg2);
}

/*
 * Copy in interrupt vector table and FIQ handler _table and _table_end
 * are symbols defined in the interrupt assembly file, at the beginning
 * and end of the table and its embedded constants.
 */
void install_interrupt_handlers(void) {
    unsigned* dst = (void*) RPI_VECTOR_START,
            * src = &_interrupt_table,
            n = &_interrupt_table_end - src;
    for (int i = 0; i < n; i++)
        dst[i] = src[i];
}

/** Safely adds a timer interrupt to the interrupt vector, returning true if successful and false otherwise. */
bool set_timer_interrupt(timer_interrupt_t interrupt) {
    if (timer_ints == MAX_TIMER_INTS) {
        return false;
    } else {
        timer_interrupts[timer_ints++] = interrupt;
        return true;
    }
}

/** Safely adds a timer interrupt to the interrupt vector, silently failing if the given pin is invalid. */
void set_gpio_interrupt(uint8_t pin, gpio_interrupt_t interrupt) {
    if(pin >= GPIO_PIN_FIRST && pin <= GPIO_PIN_LAST) {
        gpio_interrupts[pin] = interrupt;
    }
}
