#include <assert.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <limits.h>

#include <crc32.h>
#include <boot-messages.h>
#include <demand.h>
#include <x86/tty-comm.h>
#include <x86/boot-support.h>

#include "simple-boot.h"
#include "trace.h"

// unix-side bootloader: send the bytes, using the protocol.
// read/write using tty_put_uint() get_unint().
void simple_boot(int fd, const uint8_t* givenBuf, uint32_t n) {
    const uint8_t* buf = givenBuf;
    if(n % 4 != 0) {
        buf = align_file(&n, buf, 4);
    }

    if(buf != givenBuf) {
        free((void*) buf);
    }
}
