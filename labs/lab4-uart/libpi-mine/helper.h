//
// Created by jonak on 1/23/19.
//

#ifndef LAB4_UART_HELPER_H
#define LAB4_UART_HELPER_H

void setBit(volatile unsigned int* reg, unsigned char bitNum, unsigned char value);

unsigned char getBit(volatile unsigned int* reg, unsigned char bitNum);

#endif //LAB4_UART_HELPER_H
