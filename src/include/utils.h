#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>

unsigned short bytes_to_short(unsigned char *buffer);
unsigned int bytes_to_int(unsigned char *buffer);
int read_bytes(FILE *db, int offset, int no_of_bytes, unsigned char *buffer);
int *read_array_of_short(FILE *db, int offset, int size);

#endif // UTILS_H
