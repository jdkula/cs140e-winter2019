#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <boot-messages.h>

#include <demand.h>
#include <integer.h>
#include <x86/tty-comm.h>
#include "x86/boot-support.h"

// simple utility function to check that a u32 read from the
// file descriptor matches <v>.
void expect(const char* msg, int fd, uint32_t v) {
    uint32_t x = tty_get_uint(fd);
    if (x != v) {
        tty_put_uint(fd, NAK);
        panic("%s: expected %x, got %x\n", msg, v, x);
    }
}

// read entire file into buffer.  return it, write totat bytes to <size>
uint8_t* read_file(int* size, const char* name) {
    struct stat fileStat;
    if (stat(name, &fileStat) < 0) {
        panic("Stat failed");
    }
    off_t fileSize = fileStat.st_size;


    unsigned char* buf = malloc(fileSize);

    int fileFd = open(name, O_RDONLY);
    if (fileFd < 0) {
        panic("Could not open file...");
    }

    int thisRead = read(fileFd, buf, fileSize);
    assert(thisRead == fileSize);

    *size = fileSize;
    return buf;
}

unsigned char* align_file(uint32_t* size, const uint8_t* file, unsigned align_to) {
    int oldsize = *size;
    unsigned align_addition = (align_to - (*size % align_to));
    *size += align_addition == align_to ? 0 : align_addition;

    uint8_t* new = malloc(*size);

    memcpy(new, file, oldsize);

    for(int i = oldsize; i < *size; i++) {
        new[i] = 0;
    }

    return new;
}
