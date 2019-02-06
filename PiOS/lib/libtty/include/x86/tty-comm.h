//
// Created by jonak on 2/6/19.
//

#ifndef PIOS_TTY_COMM_H
#define PIOS_TTY_COMM_H

#include <integer.h>

void send_byte(int fd, uint8 b);

uint8 get_byte(int fd);

uint32 tty_get_uint(int fd);

void tty_put_uint(int fd, uint32 u) ;

#endif //PIOS_TTY_COMM_H
