#ifndef __RPI_INTERRUPT_H__
#define __RPI_INTERRUPT_H__

#include "rpi.h"

// from the valvers description.

/** @brief Bits in the Enable_Basic_IRQs register to enable various interrupts.
   See the BCM2835 ARM Peripherals manual, section 7.5 */
#define RPI_BASIC_ARM_TIMER_IRQ         (1 << 0)
#define RPI_BASIC_ARM_MAILBOX_IRQ       (1 << 1)
#define RPI_BASIC_ARM_DOORBELL_0_IRQ    (1 << 2)
#define RPI_BASIC_ARM_DOORBELL_1_IRQ    (1 << 3)
#define RPI_BASIC_GPU_0_HALTED_IRQ      (1 << 4)
#define RPI_BASIC_GPU_1_HALTED_IRQ      (1 << 5)
#define RPI_BASIC_ACCESS_ERROR_1_IRQ    (1 << 6)
#define RPI_BASIC_ACCESS_ERROR_0_IRQ    (1 << 7)

// http://xinu.mscs.mu.edu/BCM2835_Interrupt_Controller
typedef struct {
    volatile uint32 irq_basic_pending;
    volatile uint32 irq_pending_1;
    volatile uint32 irq_pending_2;
    volatile uint32 fiq_control;
    volatile uint32 enable_irqs_1;
    volatile uint32 enable_irqs_2;
    volatile uint32 enable_basic_irqs;
    volatile uint32 disable_irqs_1;
    volatile uint32 disable_irqs_2;
    volatile uint32 disable_basic_irqs;
} rpi_irq_controller_t;

volatile rpi_irq_controller_t* get_irq_controller(void);

// and this with irq_basic_pending to see if it's a timer interrupt.
#define RPI_BASIC_ARM_TIMER_IRQ         (1 << 0)

unsigned extract_byte(unsigned);

void system_enable_interrupts(void);
void system_disable_interrupts(void);

// location of these registers.
#define INTERRUPT_ENABLE_1  0x2000b210
#define INTERRUPT_ENABLE_2  0x2000b214
#define INTERRUPT_DISABLE_1 0x2000b21c
#define INTERRUPT_DISABLE_2 0x2000b220

// where the interrupt handlers go.
#define RPI_VECTOR_START  0

extern unsigned _interrupt_table;
extern unsigned _interrupt_table_end;

void install_int_handlers(void);
#endif 
