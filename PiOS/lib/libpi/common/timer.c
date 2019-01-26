#include "rpi.h"

void delay(unsigned ticks) {
    //no-op
}

unsigned timer_get_time(void) {
    return *(unsigned*) 0x20003004;
}

void delay_us(unsigned us) {
    unsigned rb = timer_get_time();
    while (1) {
        unsigned ra = timer_get_time();
        if ((ra - rb) >= us) {
            break;
        }
    }
}

void delay_ms(unsigned ms) {
    delay_us(ms * 1000);
}
