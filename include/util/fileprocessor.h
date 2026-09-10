#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <stdio.h>

int read_bytes(FILE *db, int offset, int no_of_bytes, unsigned char *buffer);
short *read_array_of_shorts(FILE *db, int offset, short size);
short *read_array_of_byteInts(FILE *db, int offset, short size);

#endif // FILEPROCESSOR_H
