#include "memory.h"
#include <stdio.h>

// malloc wrapper
void *mallox(size_t size) {
  void *p = malloc(size);
  if (p == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }

  return p;
}

// realloc wrapper
void *reallox(void *ptr, size_t size) {
  void *p = realloc(ptr, size);
  if (p == NULL) {
    perror("realloc failed");
    exit(EXIT_FAILURE);
  }

  return p;
}
