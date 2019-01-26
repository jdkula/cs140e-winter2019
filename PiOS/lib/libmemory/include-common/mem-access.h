//
// Created by jonak on 1/23/19.
//

#ifndef LAB4_UART_HELPER_H
#define LAB4_UART_HELPER_H

// *(unsigned *)addr = v;
void PUT32(unsigned addr, unsigned v);

// *(unsigned short *)addr = v;
void PUT16(unsigned addr, unsigned v);

// *(unsigned char *)addr = v;
void PUT8(unsigned addr, unsigned v);

// *(unsigned *)addr
unsigned GET32(unsigned addr);

// Get Program Counter
unsigned GETPC();

// set pc value to <addr>
void BRANCHTO(unsigned int addr);

void setBit(unsigned int* reg, unsigned char bitNum, unsigned char value);

unsigned char getBit(unsigned int* reg, unsigned char bitNum);

void put32(void* addr, unsigned data);

unsigned get32(void* addr);

#endif //LAB4_UART_HELPER_H
