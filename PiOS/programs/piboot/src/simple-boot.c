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

uint32_t get_uint_traced(int fd) {
    uint32_t recv = tty_get_uint(fd);
    trace_read32(recv);
    return recv;
}

void put_uint_traced(int fd, unsigned data) {
    trace_write32(data);
    tty_put_uint(fd, data);
}

// simple utility function to check that a u32 read from the 
// file descriptor matches <v>.
void expect(const char* msg, int fd, uint32_t v) {
    uint32_t x = get_uint_traced(fd);
    if (x != v) {
        put_uint_traced(fd, NAK);
        panic("%s: expected %x, got %x\n", msg, v, x);
    }
}

// unix-side bootloader: send the bytes, using the protocol.
// read/write using put_uint() get_unint().
void simple_boot(int fd, const uint8_t* givenCode, uint32_t n) {
    const uint8_t* code = givenCode;
    if(n % 4 != 0) {
        code = align_file(&n, code, 4);
    }
    const uint32_t* intCode = (const uint32_t*) (code);

    uint32_t nCrc = crc32(&n, 4);
    uint32_t bufCrc = crc32(code, n);

    send_byte(fd, SOH);

    expect("Pi sends ACK", fd, ACK);
    printf("Got ack.\n");

    put_uint_traced(fd, code[0]); // Version #
    put_uint_traced(fd, code[1]); // Link address

    put_uint_traced(fd, n);
    put_uint_traced(fd, bufCrc);

    expect("Pi Echoes Byte Number CRC", fd, nCrc);
    fprintf(stderr, "Byte number CRC verified... %#010x\n", nCrc);

    expect("Pi Echoes Data CRC", fd, bufCrc);
    fprintf(stderr, "Data CRC verified... %#010x\n", bufCrc);

    put_uint_traced(fd, ACK);  // We're good to go!

    expect("Pi sends ACK before code distribution", fd, ACK);

    uint32_t intBufSize = n / 4;
    fprintf(stderr, "File info: %u bytes / %u chunks\n", n, intBufSize);

    for (int i = 0; i < intBufSize; i++) {
        put_uint_traced(fd, intCode[i]);
        fprintf(stderr, "Sent %u/%u (%d%%) chunks\r", i, intBufSize, (i * 100) / intBufSize);
    }

    fprintf(stderr, "\nSent all data...\n");

    put_uint_traced(fd, EOT);
    fprintf(stderr, "Sent EOT...\n");

    expect("Pi Sends ACK", fd, ACK);
    fprintf(stderr, "Received ACK!\n");

    if(code != givenCode) {
        fprintf(stderr, "Freeing aligned code.");
        free((void*) code);
    }

    expect("Pi reads from correct area", fd, 0x10001df8);
    expect("Area is not corrupted", fd, 'H');
}
