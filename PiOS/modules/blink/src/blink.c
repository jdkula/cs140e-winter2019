#include <gpio.h>
#include <timer.h>
#include <integer.h>
#include <debug.h>
#include <rpi.h>

int module_main (void ) {
    debug_enable(26);
    debug_enable(21);
    debug_enable(20);
    debug_enable(19);
    debug_enable(16);

    uint8 times = 10;

    for(; times > 0; times--) {
        debug_on(26);

        delay(100000);
        debug_on(21);

        delay(100000);
        debug_on(20);

        delay(100000);
        debug_on(19);

        delay(100000);
        debug_on(16);

        delay(1000000);

        debug_off(26);

        delay(100000);
        debug_off(21);

        delay(100000);
        debug_off(20);

        delay(100000);
        debug_off(19);

        delay(100000);
        debug_off(16);
        
        delay(1000000);
    }

    reboot();
    return 0;
}