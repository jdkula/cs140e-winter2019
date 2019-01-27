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
#include "demand.h"
#include "support.h"

#include <crc32.h>
#include <boot-messages.h>

static void send_byte(int fd, uint8 b) {
    if (write(fd, &b, 1) < 0)
        panic("write failed in send_byte\n");
}

static uint8 get_byte(int fd) {
    uint8 b;
    int n;
    if ((n = read(fd, &b, 1)) != 1)
        panic("read failed in get_byte: expected 1 byte, got %d\n", n);
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
//        printf("< %#010x\n", u);
    return u;
}

void put_uint(int fd, uint32 u) {
    // mask not necessary.
//        printf("> %#010x\n", u);
    send_byte(fd, (u >> 0) & 0xff);
    send_byte(fd, (u >> 8) & 0xff);
    send_byte(fd, (u >> 16) & 0xff);
    send_byte(fd, (u >> 24) & 0xff);
}

// simple utility function to check that a u32 read from the 
// file descriptor matches <v>.
void expect(const char *msg, int fd, uint32 v) {
    uint32 x = get_uint(fd);
    if (x != v)
        panic("%s: expected %x, got %x\n", msg, v, x);
}

// unix-side bootloader: send the bytes, using the protocol.
// read/write using put_uint() get_unint().
void simple_boot(int fd, const uint8 *buf, uint32 n) {
    uint32 nCrc = crc32(&n, 4);
    uint32 bufCrc = crc32(buf, n);

    put_uint(fd, SOH);
    put_uint(fd, n);
    put_uint(fd, bufCrc);

    expect("Pi echoes SOH", fd, SOH);
    printf("SOH received...\n");

    expect("Pi Echoes Byte Number CRC", fd, nCrc);
    printf("Byte number CRC verified... %#010x\n", nCrc);

    expect("Pi Echoes Data CRC", fd, bufCrc);
    printf("Data CRC verified... %#010x\n", bufCrc);

    const uint32 *intBuf = (const uint32 *) (buf);
    uint32 intBufSize = n / 4;
    printf("File info: %u bytes / %u chunks\n", n, intBufSize);

    uint32 sent = 0;
    for (int i = 0; i < intBufSize; i++) {
        put_uint(fd, intBuf[i]);
        sent++;
        printf("Sent %u/%u (%d%%) chunks\r", sent, intBufSize, (sent * 100) / intBufSize);
    }

    printf("\nSent all data...\n");

    put_uint(fd, EOT);
    printf("Sent EOT...\n");

    expect("Pi Sends ACK", fd, ACK);
    printf("Received ACK!\n");
}
