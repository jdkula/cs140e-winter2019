/*
	simple timer interrupt demonstration.

      Good timer / interrupt discussion at:
 	http://www.valvers.com/open-software/raspberry-pi/step04-bare-metal-programming-in-c-pt4/

	Most of this code is from here.
*/
#include <mem-barrier.h>
#include <mem-access.h>
#include <armtimer.h>
#include <interrupts.h>
#include "rpi.h"

volatile rpi_irq_controller_t* rpi_irq_controller = (void*) 0x2000B200;

volatile rpi_irq_controller_t* get_irq_controller(void) {
    data_memory_barrier();
    data_sync_barrier();
    return rpi_irq_controller;
}

// only the lower 8 bits seem to be used --- higher ones are GPU enabled.
unsigned extract_byte(unsigned x) {
    return x & 0xff;
}

/** @brief See the documentation for the ARM side timer (Section 14 of the
    BCM2835 Peripherals PDF) */
volatile rpi_arm_timer_t* rpi_arm_timer = (void*) 0x2000B400;

volatile rpi_arm_timer_t* get_arm_timer(void) {
    data_memory_barrier();
    data_sync_barrier();
    return rpi_arm_timer;
}

void timer_interrupt_init(unsigned ncycles) {
//	install_int_handlers();

    // BCM2835 manual, section 7.5
    put32(INTERRUPT_DISABLE_1, 0xffffffff);
    put32(INTERRUPT_DISABLE_2, 0xffffffff);
    data_memory_barrier();
    data_sync_barrier();

    // Bit 52 in IRQ registers enables/disables all GPIO interrupts
#if 0
    // Bit 52 is in the second register, so subtract 32 for index
    PUT32(INTERRUPT_ENABLE_2, (1 << (52 - 32)));
      dmb();
      dsb();
#endif

    // from valvers:
    //	 Enable the timer interrupt IRQ
    get_irq_controller()->enable_basic_irqs = RPI_BASIC_ARM_TIMER_IRQ;

    /* Setup the system timer interrupt */
    /* Timer frequency = Clk/256 * 0x400 */
    // get_arm_timer()->load = 0x400;
    // get_arm_timer()->load = 0x100;
    // get_arm_timer()->load = 0x4;
    get_arm_timer()->load = ncycles;

    // Setup the ARM Timer
    get_arm_timer()->control =
            RPI_ARMTIMER_CTRL_23BIT |
            RPI_ARMTIMER_CTRL_ENABLE |
            RPI_ARMTIMER_CTRL_INT_ENABLE |
            RPI_ARMTIMER_CTRL_PRESCALE_1;

    //              RPI_ARMTIMER_CTRL_PRESCALE_256;

    // Now GPIO interrupts are enabled, but interrupts are still
    // globally disabled. Caller has to enable them when they are
    // ready.
}
