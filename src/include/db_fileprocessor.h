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
short get_page_header_size(
    FILE *database_file,
    int page_no); // INFO: No of columns = page header size - 1

unsigned short get_no_of_cells(FILE *database_file, int page_no);
void get_cell_offsets(FILE *database_file, int page_no, short **output,
                      short no_of_cells);
Column **get_columns_from_a_record(FILE *database_file, int offset,
                                   short *no_of_columns);
void free_columns(Column **columns, short no_of_columns);

void get_column_size_and_dt(FILE *database_file, short size, Column *column);

#endif // DB_FILEPROCESSOR_H
