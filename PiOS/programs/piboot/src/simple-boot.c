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

#include "support.h"
#include "trace.h"

static void send_byte(int fd, uint8 b) {
    if (write(fd, &b, 1) < 0) {
        sys_die(write, "write failed in send_byte\n");
    }
}

static uint8 get_byte(int fd) {
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
static uint32 get_uint(int fd) {
    uint32 u = get_byte(fd);
    u |= get_byte(fd) << 8;
    u |= get_byte(fd) << 16;
    u |= get_byte(fd) << 24;
    trace_read32(u);
    return u;
}

void put_uint(int fd, uint32 u) {
    // mask not necessary.
//    fprintf(stderr, "> %#010x\n", u);
    send_byte(fd, (u >> 0) & 0xff);
    send_byte(fd, (u >> 8) & 0xff);
    send_byte(fd, (u >> 16) & 0xff);
    send_byte(fd, (u >> 24) & 0xff);
    trace_write32(u);
}

// simple utility function to check that a u32 read from the 
// file descriptor matches <v>.
void expect(const char* msg, int read_fd, int write_fd, uint32 v) {
    uint32 x = get_uint(read_fd);
    if (x != v) {
        put_uint(write_fd, NAK);
        panic("%s: expected %x, got %x\n", msg, v, x);
    }
}

// unix-side bootloader: send the bytes, using the protocol.
// read/write using put_uint() get_unint().
void simple_boot(int read_fd, int write_fd, const uint8* buf, uint32 n) {
    uint32 nCrc = crc32(&n, 4);
    uint32 bufCrc = crc32(buf, n);

    put_uint(write_fd, SOH);
    put_uint(write_fd, n);
    put_uint(write_fd, bufCrc);

    expect("Pi echoes SOH", read_fd, write_fd, SOH);
    fprintf(stderr, "SOH received...\n");

    expect("Pi Echoes Byte Number CRC", read_fd, write_fd, nCrc);
    fprintf(stderr, "Byte number CRC verified... %#010x\n", nCrc);

    expect("Pi Echoes Data CRC", read_fd, write_fd, bufCrc);
    fprintf(stderr, "Data CRC verified... %#010x\n", bufCrc);

    put_uint(write_fd, ACK);  // We're good to go!

    const uint32* intBuf = (const uint32*) (buf);
    uint32 intBufSize = n / 4;
    fprintf(stderr, "File info: %u bytes / %u chunks\n", n, intBufSize);

    for (int i = 0; i < intBufSize; i++) {
        put_uint(write_fd, intBuf[i]);
        fprintf(stderr, "Sent %u/%u (%d%%) chunks\r", i, intBufSize, (i * 100) / intBufSize);
    }

    fprintf(stderr, "\nSent all data...\n");

    put_uint(write_fd, EOT);
    fprintf(stderr, "Sent EOT...\n");

    expect("Pi Sends ACK", read_fd, write_fd, ACK);
    fprintf(stderr, "Received ACK!\n");
}
