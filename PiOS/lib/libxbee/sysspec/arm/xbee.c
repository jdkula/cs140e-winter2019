//
// Created by jonak on 3/9/19.
//

#include <gpio.h>

void xbee_init(uint8_t reset_pin) {
    gpio_init();

    gpio_set_pullup(reset_pin);


}