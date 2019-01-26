#include <gpio.h>
#include <timer.h>

/*
 * very simple bootloader.  more robust than xmodem.   (that code seems to
 * have bugs in terms of recovery with inopportune timeouts.)
 */

void notmain ( void ) {
    int led = 20;

    gpio_set_output(led);
    for(int i = 0; i < 100; i++) {
        gpio_write(led, 1);
        delay(1000000);
        gpio_write(led, 0);
        delay(1000000);
    }
}