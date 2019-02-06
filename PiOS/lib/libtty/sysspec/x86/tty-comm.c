//
// Created by jonak on 2/6/19.
//

#include <demand.h>
#include <unistd.h>
#include <stdio.h>

#include "x86/tty-comm.h"

void send_byte(int fd, uint8 b) {
    if (write(fd, &b, 1) < 0) {
        sys_die(write, "write failed in send_byte\n");
    }
}

uint8 get_byte(int fd) {
    uint8 b;
    int n;
    if ((n = read(fd, &b, 1)) != 1) {
        panic("read failed in get_byte: expected 1 byte, got %d\n", n);
    }
    return b;
}

// NOTE: the other way to do is to assign these to a char array b and
//	return *(uint32)b
// however, the compiler doesn't have to align b to what uint32
// requires, so this can go awry.  easier to just do the simple way.
// we do with |= to force get_byte to get called in the right order
// 	(get_byte(fd) | get_byte(fd) << 8 ...)
// isn't guaranteed to be called in that order b/c | is not a seq point.
uint32 tty_get_uint(int fd) {
    uint32 u = get_byte(fd);
    u |= get_byte(fd) << 8;
    u |= get_byte(fd) << 16;
    u |= get_byte(fd) << 24;
    return u;
}

void tty_put_uint(int fd, uint32 u) {
    // mask not necessary.
//    fprintf(stderr, "> %#010x\n", u);
    send_byte(fd, (u >> 0) & 0xff);
    send_byte(fd, (u >> 8) & 0xff);
    send_byte(fd, (u >> 16) & 0xff);
    send_byte(fd, (u >> 24) & 0xff);
}


