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

#include "simple-boot.h"
#include "support.h"
#include "trace.h"

uint32 get_uint_traced(int fd) {
    uint32 recv = tty_get_uint(fd);
    trace_read32(recv);
    return recv;
}

void put_uint_traced(int fd, unsigned data) {
    trace_write32(data);
    tty_put_uint(fd, data);
}

// simple utility function to check that a u32 read from the 
// file descriptor matches <v>.
void expect(const char* msg, int fd, uint32 v) {
    uint32 x = get_uint_traced(fd);
    if (x != v) {
        put_uint_traced(fd, NAK);
        panic("%s: expected %x, got %x\n", msg, v, x);
    }
}

// unix-side bootloader: send the bytes, using the protocol.
// read/write using put_uint() get_unint().
void simple_boot(int fd, const uint8* givenBuf, uint32 n) {
    const uint8* buf = givenBuf;
    if(n % 4 != 0) {
        buf = align_file(&n, buf, 4);
    }

    uint32 nCrc = crc32(&n, 4);
    uint32 bufCrc = crc32(buf, n);

    put_uint_traced(fd, SOH);
    put_uint_traced(fd, n);
    put_uint_traced(fd, bufCrc);

    expect("Pi echoes SOH", fd, SOH);
    fprintf(stderr, "SOH received...\n");

    expect("Pi Echoes Byte Number CRC", fd, nCrc);
    fprintf(stderr, "Byte number CRC verified... %#010x\n", nCrc);

    expect("Pi Echoes Data CRC", fd, bufCrc);
    fprintf(stderr, "Data CRC verified... %#010x\n", bufCrc);

    put_uint_traced(fd, ACK);  // We're good to go!

    const uint32* intBuf = (const uint32*) (buf);
    uint32 intBufSize = n / 4;
    fprintf(stderr, "File info: %u bytes / %u chunks\n", n, intBufSize);

    for (int i = 0; i < intBufSize; i++) {
        put_uint_traced(fd, intBuf[i]);
        fprintf(stderr, "Sent %u/%u (%d%%) chunks\r", i, intBufSize, (i * 100) / intBufSize);
    }

    fprintf(stderr, "\nSent all data...\n");

    put_uint_traced(fd, EOT);
    fprintf(stderr, "Sent EOT...\n");

    expect("Pi Sends ACK", fd, ACK);
    fprintf(stderr, "Received ACK!\n");

    if(buf != givenBuf) {
        free((void*) buf);
    }
}
