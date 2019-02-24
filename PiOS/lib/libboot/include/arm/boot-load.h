//
// Created by jonak on 2/20/19.
//

#ifndef PIOS_PIBOOT_H
#define PIOS_PIBOOT_H

#include <integer.h>

#define LAST_USED_ADDRESSES 0x9000000
#define MAX_ADDRESS        0x16000000

uint32_t load_code(void);

#endif //PIOS_PIBOOT_H
