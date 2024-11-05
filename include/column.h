#ifndef COLUMN_H
#define COLUMN_H

#include <stdio.h>

typedef struct {
  int size; // number of bytes
  char *datatype;
} Column;

Column **get_columns_from_a_record(FILE *database_file, int offset,
                                   short *no_of_columns);

void free_columns(Column **columns, short no_of_columns);

#endif // COLUMN_H
