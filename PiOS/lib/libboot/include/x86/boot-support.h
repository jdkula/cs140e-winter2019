#include <integer.h>

unsigned char* read_file(int* size, const char* name);
unsigned char* align_file(uint32_t* size, const uint8_t* file, unsigned align_to);
void expect(const char* msg, int fd, uint32_t v);