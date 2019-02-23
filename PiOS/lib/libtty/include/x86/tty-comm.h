//
// Created by jonak on 2/6/19.
//

#ifndef PIOS_TTY_COMM_H
#define PIOS_TTY_COMM_H

#include <integer.h>

void tty_send_byte(int fd, uint8_t b);

uint8_t tty_get_byte(int fd);

uint32_t tty_get_uint(int fd);

void tty_put_uint(int fd, uint32_t u);

#endif //PIOS_TTY_COMM_H
