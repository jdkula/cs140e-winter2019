#include <gpio.h>
#include <timer.h>
#include <integer.h>
#include <debug.h>
#include <rpi.h>
#include <uart.h>
#include <printf.h>
#include <aux.h>

const uint8_t THE_PIN = GPIO_ACT;

int notmain(void) {
    gpio_set_output(THE_PIN);

    uint8_t times = 10;

    for (; times > 0; times--) {
        gpio_write(THE_PIN, HIGH);
        delay(1000000);
        gpio_write(THE_PIN, LOW);
        delay(1000000);
    }

    printk("DONE!!!\n");

    reboot();
    return 0;
}