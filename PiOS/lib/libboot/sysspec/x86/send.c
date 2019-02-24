//
// Created by jonak on 2/22/19.
//


#include <x86/boot-support.h>
#include <demand.h>
#include <x86/tty-comm.h>
#include <assert.h>
#include <boot-messages.h>
#include <crc32.h>

#define returnIfNot(fd, result) do {        \
    uint32_t got = tty_get_uint(fd);        \
    if(result != got) {                     \
        return got;                         \
    }                                       \
} while(0);

int send_program(int fd, const char* name) {
    int nbytes;

    // from homework.
    unsigned* code = (void*) read_file(&nbytes, name);
    assert(nbytes % 4 == 0);

    tty_send_byte(fd, SOH);

    returnIfNot(fd, ACK);
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

    if (backNumber != numberCrc || backMsgCrc != msgCrc) {
        tty_put_uint(fd, NAK);
        printf("rip, didn't match\n");
        return BAD_CKSUM;
    }

    tty_put_uint(fd, ACK);

    returnIfNot(fd, ACK);

    for (int i = 0; i < (nbytes / 4); i += 1) {
        tty_put_uint(fd, code[i]);
    }
    printf("sent code.\n");

    tty_put_uint(fd, EOT);
    printf("sent eot.\n");

    returnIfNot(fd, ACK);
    printf("done.\n");

    return 0;
}