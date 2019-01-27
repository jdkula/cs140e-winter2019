#include <gpio.h>
#include <timer.h>
#include <integer.h>
#include <debug.h>
#include <rpi.h>

int module_main (void ) {
    gpio_set_output(26);
    gpio_set_output(21);
    gpio_set_output(20);
    gpio_set_output(19);
    gpio_set_output(16);

    uint8 times = 10;

    for(; times > 0; times--) {
        debug_wave();
        delay(1000000);
    }

    reboot();
    return 0;
}