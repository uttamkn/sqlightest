#include "fileprocessor.h"
#include "conversion.h"
#include "memory.h"
#include <stdlib.h>

int read_bytes(FILE *db, int offset, int no_of_bytes, unsigned char *buffer) {
  if (buffer == NULL) {
    perror("Buffer is NULL");
    return -1;
  }

  if (fseek(db, offset, SEEK_SET) != 0) {
    perror("fseek failed");
    return -1;
  }

  size_t bytesRead = fread(buffer, 1, no_of_bytes, db);
  if (bytesRead < no_of_bytes) {
    if (feof(db)) {
      perror("End of file reached");
    } else if (ferror(db)) {
      perror("fread failed");
    }
    return -1;
  }

  return 0;
}

short *read_array_of_shorts(FILE *db, int offset, short size) {
  short *output = (short *)mallox(sizeof(short) * size);

  for (int i = 0; i < size; ++i) {
    int current_offset = offset + 2 * i;

    unsigned char buffer[2];
    if (read_bytes(db, current_offset, 2, buffer) != 0) {
      free(output);
      perror("Failed to read the bytes");
      return NULL;
    }

    output[i] = bytes_to_short(buffer);
  }

  return output;
}

short *read_array_of_byteInts(FILE *db, int offset, short size) {
  short *output = (short *)mallox(sizeof(short) * size);

  for (int i = 0; i < size; ++i) {
    int current_offset = offset + i;

    unsigned char buffer[1];
    if (read_bytes(db, current_offset, 1, buffer) != 0) {
      free(output);
      perror("Failed to read the bytes");
      return NULL;
    }

    output[i] = (short)buffer[0];
  }

  return output;
}
