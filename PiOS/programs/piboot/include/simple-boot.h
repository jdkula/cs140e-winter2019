//
// Created by jonak on 1/27/19.
//

#ifndef PIOS_SIMPLE_BOOT_H
#define PIOS_SIMPLE_BOOT_H

#include <integer.h>

void simple_boot(int read_fd, int write_fd, const uint8 *buf, uint32 n);

#endif //PIOS_SIMPLE_BOOT_H
