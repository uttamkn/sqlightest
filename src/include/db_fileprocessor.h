#ifndef DB_FILEPROCESSOR_H
#define DB_FILEPROCESSOR_H

#include <stdio.h>

typedef struct {
  int size; // number of bytes
  char *datatype;
} Column;

unsigned short get_page_size(FILE *database_file);
unsigned int get_no_of_tables(FILE *database_file);

int get_page_offset(int page_no);
int get_page_header_size(
    FILE *database_file,
    int page_no); // INFO: No of columns = page header size - 1

unsigned short get_no_of_cells(FILE *database_file, int page_no);
void get_cell_offsets(FILE *database_file, int page_no, int **output,
                      int no_of_cells);

#endif // DB_FILEPROCESSOR_H
