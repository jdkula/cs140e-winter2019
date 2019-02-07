//
// Created by jonak on 2/6/19.
//

#ifndef PIOS_STDLIB_H
#define PIOS_STDLIB_H

#include <integer.h>

#define aligned(ptr, n)  ((unsigned)ptr % n == 0)
#define aligned4(ptr)  aligned(ptr,4)

void *memset(void *_p, int c, size_t n);
int memcmp(const void *_s1, const void *_s2, size_t nbytes);
void *memcpy(void *dst, const void *src, size_t nbytes);

#endif //PIOS_STDLIB_H
