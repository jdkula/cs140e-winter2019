//
// Created by jonak on 1/26/19.
//

#include <integer.h>
#include "simple-boot.h"

uint32 crc32(const void* buf, unsigned size) {
    const uint8* p;
    uint32 crc = 0;

    p = buf;
    crc = crc ^ ~0U;

    while (size--)
        crc = crc32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);

    return crc ^ ~0U;
}