//
// Created by jonak on 3/10/19.
//

#include <interrupts.h>
#include <demand.h>
#include <gpio.h>
#include <arm.h>

void interrupt_vector(unsigned pc) {
    volatile rpi_irq_controller_t* irq = get_irq_controller();

    uint32_t pending = irq->irq_basic_pending;

//	if(pending & RPI_BASIC_REG1_PENDING_IRQ) {
//
//	}

    if (pending & RPI_BASIC_ARM_TIMER_IRQ) {
        for (uint8_t i = 0; i < timer_ints; i++) {
            timer_interrupts[i]();
        }
        get_arm_timer()->irq_clear = 1;
    }

    if (pending & RPI_BASIC_REG2_PENDING_IRQ) {
        uint32_t pending2 = irq->irq_pending_2;

        if ((pending2 >> 20) & 1) { //  gpio_int[3]
            for (uint8_t pin = GPIO_PIN_FIRST; pin <= GPIO_PIN_LAST; pin++) {
                if (gpio_check_and_clear_event(pin) && gpio_interrupts[pin]) {
                    gpio_interrupts[pin](pin);
                }
            }
        }
    }
}

#define UNHANDLED(msg, r) \
    panic("ERROR: unhandled exception <%s> at PC=%x\n", msg,r)

void fast_interrupt_vector(unsigned pc) {
    UNHANDLED("fast", pc);
}

void software_interrupt_vector(unsigned pc) {
    UNHANDLED("soft interrupt", pc);
}

void reset_vector(unsigned pc) {
    UNHANDLED("reset vector", pc);
}

void undefined_instruction_vector(unsigned pc) {
    UNHANDLED("undefined instruction", pc);
}

void prefetch_abort_vector(unsigned pc) {
    UNHANDLED("prefetch abort", pc);
}

void data_abort_vector(unsigned pc) {
    UNHANDLED("data abort", pc);
}

