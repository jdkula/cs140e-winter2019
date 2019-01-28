#include <gpio.h>
#include <timer.h>
#include <integer.h>
#include <debug.h>
#include <rpi.h>

const uint8 THE_PIN = GPIO_PWR;

int module_main (void ) {
    gpio_set_output(THE_PIN);

    uint8 times = 10;

    for(; times > 0; times--) {
        gpio_write(THE_PIN, HIGH);
        delay(100000);
        gpio_write(THE_PIN, LOW);
        delay(100000);
    }

    reboot();
    return 0;
}