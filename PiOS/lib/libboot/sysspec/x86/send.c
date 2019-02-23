//
// Created by jonak on 2/22/19.
//


#include <x86/support.h>
#include <demand.h>
#include <x86/tty-comm.h>
#include <assert.h>
#include <boot-messages.h>
#include <crc32.h>

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

    if (backNumber != numberCrc || backMsgCrc != msgCrc) {
        tty_put_uint(fd, NAK);
        printf("rip, didn't match\n");
        return;
    }

    tty_put_uint(fd, ACK);

    printf("about to expect ack\n");
    unsigned back = tty_get_uint(fd);
    printf("got back %d, expected %d\n", back, ACK);
    if (back != ACK) {
        exit(1);
    }

    for (int i = 0; i < (nbytes / 4); i += 1) {
        tty_put_uint(fd, code[i]);
    }
    printf("sent code.\n");

    tty_put_uint(fd, EOT);
    printf("sent eot.\n");

    expect("ack", fd, ACK);
    printf("done.\n");
}