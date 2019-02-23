//
// Created by jonak on 1/27/19.
//

#ifndef PIOS_SIMPLE_BOOT_H
#define PIOS_SIMPLE_BOOT_H

#include <integer.h>

void simple_boot(int fd, const uint8_t* givenBuf, uint32_t n);
void send_program(int fd, const char* name);


#endif //PIOS_SIMPLE_BOOT_H
