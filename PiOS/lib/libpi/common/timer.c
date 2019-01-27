#include <integer.h>
#include "rpi.h"

void delay(uint32 ticks) {
    for(; ticks > 0; ticks--) {
        dummy(0);
    }
}

uint32 timer_get_time(void) {
    return *(uint32*) 0x20003004;
}

void delay_us(uint32 us) {
    uint32 rb = timer_get_time();
    while (1) {
        uint32 ra = timer_get_time();
        if ((ra - rb) >= us) {
            break;
        }
    }
}

void delay_ms(uint32 ms) {
    delay_us(ms * 1000);
}
