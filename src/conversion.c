#include "conversion.h"

unsigned short bytes_to_short(unsigned char *buffer) {
  return (buffer[1]) | (buffer[0] << 8);
}

unsigned int bytes_to_int(unsigned char *buffer) {
  return (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | (buffer[3]);
}
