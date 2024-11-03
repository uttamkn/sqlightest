#include "../include/utils.h"

unsigned short convertBytesToShort(unsigned char *buffer) {
  return (buffer[1]) | (buffer[0] << 8);
}

unsigned int convertBytesToInt(unsigned char *buffer) {
  return (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | (buffer[3]);
}

int readBytes(FILE *db, int offset, int no_of_bytes, unsigned char *buffer) {
  if (buffer == NULL) {
    fprintf(stderr, "Buffer is NULL\n");
    return -1;
  }

  if (fseek(db, offset, SEEK_SET) != 0) {
    perror("fseek failed");
    return -1;
  }

  size_t bytesRead = fread(buffer, 1, no_of_bytes, db);
  if (bytesRead < no_of_bytes) {
    if (feof(db)) {
      fprintf(stderr, "End of file reached\n");
    } else if (ferror(db)) {
      perror("fread failed");
    }
    return -1;
  }

  return 0;
}
