#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>

unsigned short convertBytesToShort(unsigned char *buffer);
unsigned int convertBytesToInt(unsigned char *buffer);
int readBytes(FILE *db, int offset, int no_of_bytes, unsigned char *buffer);

#endif // UTILS_H
