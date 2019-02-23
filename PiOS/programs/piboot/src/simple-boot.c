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


void send_program(int fd, const char* name) {
    int nbytes;

    // from homework.
    unsigned* code = (void*) read_file(&nbytes, name);
    assert(nbytes % 4 == 0);

    tty_send_byte(fd, SOH);

    expect("ack", fd, ACK);
    printf("got ACK\n");

    tty_put_uint(fd, code[0]);
    tty_put_uint(fd, code[1]);
    printf("put code\n");

    unsigned msgCrc = crc32(code, nbytes);
    unsigned numberCrc = crc32(&nbytes, 4);
    printf("got crc\n");

    tty_put_uint(fd, nbytes);
    tty_put_uint(fd, msgCrc);
    printf("put info\n");

    unsigned backNumber = tty_get_uint(fd);
    unsigned backMsgCrc = tty_get_uint(fd);
    printf("checking %d == %d and %d == %d\n", backNumber, numberCrc, backMsgCrc, msgCrc);

    if(backNumber != numberCrc || backMsgCrc != msgCrc) {
        tty_put_uint(fd, NAK);
        printf("rip, didn't match\n");
        return;
    }

    tty_put_uint(fd, ACK);

    printf("about to expect ack\n");
    unsigned back = tty_get_uint(fd);
    printf("got back %d, expected %d\n", back, ACK);
    if(back != ACK) {
        exit(1);
    }

    for(int i = 0; i < (nbytes / 4); i += 1) {
        tty_put_uint(fd, code[i]);
    }
    printf("sent code.\n");

    tty_put_uint(fd, EOT);
    printf("sent eot.\n");

    expect("ack", fd, ACK);
    printf("done.\n");
}

// unix-side bootloader: send the bytes, using the protocol.
// read/write using tty_put_uint() get_unint().
void simple_boot(int fd, const uint8_t* givenBuf, uint32_t n) {
    const uint8_t* buf = givenBuf;
    if(n % 4 != 0) {
        buf = align_file(&n, buf, 4);
    }

    uint32_t nCrc = crc32(&n, 4);
    uint32_t bufCrc = crc32(buf, n);

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

    const uint32_t* intBuf = (const uint32_t*) (buf);
    uint32_t intBufSize = n / 4;
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
