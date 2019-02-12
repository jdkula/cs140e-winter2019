#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "support.h"
#include "integer.h"
#include "demand.h"

// read entire file into buffer.  return it, write total bytes to <size>
uint8* read_file(int* size, const char* name) {
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

/**
 * Creates a new "file" allocated on the heap, which is a copy of of the given "file,"
 * aligned to the given number of bytes. The any alignment bytes are placed at the end
 * and are initialized to 0. [size] is updated to contain the new size.
 */
unsigned char* align_file(uint32* size, const uint8* file, unsigned align_to) {
    int oldsize = *size;

    // The additional mod ensures that if align_addition == align_to, then align_addition == 0.
    unsigned align_addition = (align_to - (*size % align_to)) % align_to;
    *size += align_addition;

    uint8* copy = malloc(*size);

    memcpy(copy, file, oldsize);
    memset((copy + oldsize), 0, (*size - oldsize));

    return copy;
}

#define _SVID_SOURCE

#include <dirent.h>

const char* ttyusb_prefixes[] = {
        "ttyUSB",    // linux
        "tty.SLAB_USB", // mac os
        NULL
};

/** Given a directory entry, determines if it could be a TTY device. */
int filter(const struct dirent* info) {
    int i = 0;
    while (ttyusb_prefixes[i] != 0) {
        if (strncmp(ttyusb_prefixes[i], info->d_name, strlen(ttyusb_prefixes[i])) == 0) {
            return 1;
        }
        i++;
    }

    return 0;
}

/** Given a directory entry list, frees each entry and the list. */
void clear_namelist(struct dirent** namelist, int numEntries) {
    for (int i = 0; i < numEntries; i++) {
        free(namelist[i]);
    }
    free(namelist);
}

// open the TTY-usb device:
//	- use <scandir> to find a device with a prefix given by ttyusb_prefixes
//	- returns an open fd to it
// 	- write the absolute path into <pathname> if it wasn't already
//	  given.
int open_tty(const char** portname) {
    struct dirent** namelist;
    int numEntries = scandir("/dev", &namelist, filter, alphasort);
    if (numEntries == 0) return -1;

    char* out = malloc(PATH_MAX);
    strcpy(out, "/dev/");
    strcat(out, namelist[0]->d_name);
    clear_namelist(namelist, numEntries);

    *portname = out;

    return open(*portname, O_RDWR | O_NOCTTY | O_SYNC);
}

