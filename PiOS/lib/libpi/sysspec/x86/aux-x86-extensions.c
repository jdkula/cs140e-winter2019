//
// Created by jonak on 2/6/19.
//

#include "x86/aux-x86-extensions.h"

int rpi_fd = -1;

void aux_init(int fd) {
    rpi_fd = fd;
}
