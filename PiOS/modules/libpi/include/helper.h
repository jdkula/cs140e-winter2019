//
// Created by jonak on 1/23/19.
//

#ifndef LAB4_UART_HELPER_H
#define LAB4_UART_HELPER_H

void setBit(unsigned int* reg, unsigned char bitNum, unsigned char value);

unsigned char getBit(unsigned int* reg, unsigned char bitNum);

void put32(unsigned* addr, unsigned data);
unsigned get32(unsigned* addr);

#endif //LAB4_UART_HELPER_H
