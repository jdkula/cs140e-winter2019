/*
 * sonar, hc-sr04
 *
 * the comments on the sparkfun product page might be helpful.
 *
 * Pay attention to the voltages on:
 *	- Vcc
 *	- Vout.
 * 
 * When circuit is "open" it can be noisy --- is it high or low when open?
 * If high need to configure a pin to be a pullup, if low, pulldown to reject
 * noise.
 */
#include <compat/dawson.h>
#include "rpi.h"

#define ECHO 5
#define TRIG 20
#define LED  21

void notmain(void) {
    uart_init();

    printk("starting sonar!\n");

    gpio_set_pulldown(ECHO);

    gpio_set_input(ECHO);
    gpio_set_output(TRIG);


    // put your code here.
    //
    // you'll need to:
    // 	1. init the device (pay attention to time delays here)
    //
    //	2. do a send (again, pay attention to any needed time
    // 	delays)
    //
    //	3. measure how long it takes and compute round trip
    //	by converting that time to distance using the datasheet
    // 	formula
    //
    // 	4. use the code in gpioextra.h and then replace it with your
    //	own (using the broadcom pdf in the docs/ directory).
    //

    int cnt = 0;
    while (cnt < 5000) {
        gpio_write(TRIG, 1);
        delay_us(11);
        gpio_write(TRIG, 0);

        unsigned timeout = timer_get_time() + 1000000;
        while (gpio_read(ECHO) == 0 && timer_get_time() < timeout);

        int start = timer_get_time();

        while (gpio_read(ECHO) != 0 && timer_get_time() < timeout);

        if (timer_get_time() >= timeout) {
            printk("echo lost...\n");
            continue;
        }

        int duration = timer_get_time() - start;

        int wholepart = duration / 148;

        int decpart = ((duration * 100) / 148) - (wholepart * 100);

        printk("GOT - %d.%d\n", wholepart, decpart);
        cnt++;
        delay_ms(100);
    }


    // troubleshooting:
    //	1. readings can be noisy --- you may need to require multiple
    //	high (or low) readings before you decide to trust the
    // 	signal.
    //
    // 	2. there are conflicting accounts of what value voltage you
    //	need for Vcc.
    //
    // 	3. the initial 3 page data sheet you'll find sucks; look for
    // 	a longer one.

    printk("stopping sonar !\n");
    clean_reboot();
}
