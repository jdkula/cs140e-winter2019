/* 
 * interrupts-c.c: some interrupt support code.  Default handlers, 
 * interrupt installation.
 */
#include <gpio.h>
#include <mem-access.h>
#include "rpi.h"
#include "interrupts.h"
#include "demand.h"

gpio_interrupt_t gpio_interrupts[GPIO_PIN_LAST] = {0};
timer_interrupt_t timer_interrupts[128] = {0};
uint8_t timer_ints = 0;

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

bool set_timer_interrupt(timer_interrupt_t interrupt) {
    if (timer_ints == 127) {
        return false;
    } else {
        timer_interrupts[timer_ints++] = interrupt;
        return true;
    }
}

bool gpio_interrupt_is_set(uint8_t pin) {
    if(pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST) return false;
    return gpio_interrupts[pin] != 0;
}

void set_gpio_interrupt(uint8_t pin, gpio_interrupt_t interrupt) {
    if(pin >= GPIO_PIN_FIRST && pin <= GPIO_PIN_LAST) {
        gpio_interrupts[pin] = interrupt;
    }
}
