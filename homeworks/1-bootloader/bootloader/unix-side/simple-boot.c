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

#define __SIMPLE_IMPL__

#include "simple-boot.h"
#include "integer.h"
#include "support.h"
#include "trace.h"

#include "demand.h"

void send_byte(int fd, uint8 b) {
    if (write(fd, &b, 1) < 0) {
        sys_die(write, "write failed in send_byte\n");
    }
}

uint8 get_byte(int fd) {
    uint8 b;
    int n;
    if ((n = read(fd, &b, 1)) != 1) {
        exit(1);
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
    send_byte(fd, (u >> 0) & 0xff);
    send_byte(fd, (u >> 8) & 0xff);
    send_byte(fd, (u >> 16) & 0xff);
    send_byte(fd, (u >> 24) & 0xff);
}

/** Receives an integer from the Raspberry Pi, and gives a trace message that can be replayed. */
uint32 get_uint_traced(int fd) {
    uint32 recv = tty_get_uint(fd);
    trace_read32(recv);
    return recv;
}

/** Sends an integer to the Raspberry Pi, and gives a trace message that can be replayed. */
void put_uint_traced(int fd, uint32 data) {
    trace_write32(data);
    tty_put_uint(fd, data);
}

// simple utility function to check that a u32 read from the 
// file descriptor matches <v>.
void expect(const char* msg, int fd, uint32 v) {
    uint32 x = get_uint_traced(fd);
    if (x != v) {
        put_uint_traced(fd, NAK);
        exit(1);
    }
}

// unix-side bootloader: send the bytes, using the protocol.
void simple_boot(int fd, const uint8* givenBuf, uint32 n) {
    const uint8* buf = givenBuf;        // Align the file if need be.
    if (n % 4 != 0) {
        buf = align_file(&n, buf, 4);
    }

    uint32 nCrc = crc32(&n, 4);         // Calculate hashes of the file and size number.
    uint32 bufCrc = crc32(buf, n);

    put_uint_traced(fd, SOH);           // Let the Pi know we're starting a transmission.
    put_uint_traced(fd, n);             // Send the number of bytes, and the file's CRC.
    put_uint_traced(fd, bufCrc);

    expect("Pi echoes SOH", fd, SOH);   // Verify that the Pi is responding in good form.
    fprintf(stderr, "SOH received...\n");

    expect("Pi Echoes Byte Number CRC", fd, nCrc);  // Verify that there was no corruption in the above transmission.
    fprintf(stderr, "Byte number CRC verified... %#010x\n", nCrc);

    expect("Pi Echoes Data CRC", fd, bufCrc);
    fprintf(stderr, "Data CRC verified... %#010x\n", bufCrc);

    put_uint_traced(fd, ACK);  // We're good to go at this point!

    const uint32* intBuf = (const uint32*) (buf);
    uint32 intBufSize = n / 4;
    fprintf(stderr, "File info: %u bytes / %u chunks\n", n, intBufSize);

    for (int i = 0; i < intBufSize; i++) {
        put_uint_traced(fd, intBuf[i]);     // Send all chunks to the Pi. Also, output our progress so far!
        fprintf(stderr, "Sent %u/%u (%d%%) chunks\r", i, intBufSize, (i * 100) / intBufSize);
    }

    fprintf(stderr, "\nSent all data...\n");

    put_uint_traced(fd, EOT);   // Let the Pi know we're done.
    fprintf(stderr, "Sent EOT...\n");

    expect("Pi Sends ACK", fd, ACK);  // The Pi lets us know everything's alright.
    fprintf(stderr, "Received ACK!\n");

    if (buf != givenBuf) {  // If we aligned the file, free our copy.
        free((void*) buf);
    }
}
