/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: interrupts.h
 * --------------
 * Provides methods to initialize, enable, and disable
 * interrupts of various types.
 *
 * Also defines several structs that represent interrupt
 * controllers as defined in the BCM manual.
 *
 * Also defines values of various interrupts that can be OR'd
 * together when calling initialize/enable/disable_interrupts.
 */

#ifndef __RPI_INTERRUPT_H__
#define __RPI_INTERRUPT_H__

#include "rpi.h"
#include "gpio.h"

typedef void (*gpio_interrupt_t)(uint8_t pin);
typedef void (*timer_interrupt_t)();

uint32_t cspr_read();
void int_setup_stack();

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
#define RPI_BASIC_REG1_PENDING_IRQ      (1 << 8)
#define RPI_BASIC_REG2_PENDING_IRQ      (1 << 9)

/** Bits in interrupt register 1 */
#define RPI_INTREG1_AUX_IRQ (1 << 29)

/** Bits in interrupt register 2 */
#define RPI_INTREG2_I2C_SPI_SLV_IRQ   (1 << (43 - 32))
#define RPI_INTREG2_PWA0_IRQ          (1 << (45 - 32))
#define RPI_INTREG2_PWA1_IRQ          (1 << (46 - 32))
#define RPI_INTREG2_SMI_IRQ           (1 << (48 - 32))
#define RPI_INTREG2_GPIO0_IRQ         (1 << (49 - 32))
#define RPI_INTREG2_GPIO1_IRQ         (1 << (50 - 32))
#define RPI_INTREG2_GPIO2_IRQ         (1 << (51 - 32))
#define RPI_INTREG2_GPIO3_IRQ         (1 << (52 - 32))
#define RPI_INTREG2_GPIO_ALL_IRQ      RPI_INTREG2_GPIO3_IRQ
#define RPI_INTREG2_I2C_IRQ           (1 << (53 - 32))
#define RPI_INTREG2_SPI_IRQ           (1 << (54 - 32))
#define RPI_INTREG2_PCM_IRQ           (1 << (55 - 32))
#define RPI_INTREG2_UART_IRQ          (1 << (57 - 32))

extern gpio_interrupt_t gpio_interrupts[GPIO_PIN_LAST];
extern timer_interrupt_t timer_interrupts[128];
extern uint8_t timer_ints;


// http://xinu.mscs.mu.edu/BCM2835_Interrupt_Controller
typedef struct {
    volatile uint32_t irq_basic_pending;
    volatile uint32_t irq_pending_1;
    volatile uint32_t irq_pending_2;
    volatile uint32_t fiq_control;
    volatile uint32_t enable_irqs_1;
    volatile uint32_t enable_irqs_2;
    volatile uint32_t enable_basic_irqs;
    volatile uint32_t disable_irqs_1;
    volatile uint32_t disable_irqs_2;
    volatile uint32_t disable_basic_irqs;
} rpi_irq_controller_t;

volatile rpi_irq_controller_t* get_irq_controller(void);

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

void install_interrupt_handlers(void);
void initialize_interrupts(uint32_t basic, uint32_t reg1, uint32_t reg2);
void disable_interrupts(uint32_t basic, uint32_t reg1, uint32_t reg2);
void enable_interrupts(uint32_t basic, uint32_t reg1, uint32_t reg2);
void set_gpio_interrupt(uint8_t pin, gpio_interrupt_t interrupt);
bool gpio_interrupt_is_set(uint8_t pin);
bool set_timer_interrupt(timer_interrupt_t interrupt);
#endif 
