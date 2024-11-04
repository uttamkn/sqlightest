#include "../include/db_fileprocessor.h"
#include "../include/constants.h"
#include "../include/utils.h"
#include <stdlib.h>
#include <string.h>

unsigned short get_page_size(FILE *database_file) {
  unsigned char buffer[2];
  if (read_bytes(database_file, PAGE_SIZE_OFFSET, 2, buffer) == 0) {
    return bytes_to_short(buffer);
  }

  perror("Failed to read the page size");
  return -1;
}

unsigned int get_no_of_tables(FILE *database_file) {
  // WARN: This function is not implemented properly yet (most of the time the
  // number of cells will be equal to the number of tables so number of cells is
  // being returned for now)
  return get_no_of_cells(database_file, 1);
}

int get_page_offset(int page_no) {
  int current_page_offset = 100; // offset for page 1
  if (page_no != 1) {
    current_page_offset = DEFAULT_PAGE_SIZE * (page_no - 1);
  }

  return current_page_offset;
}

short get_page_header_size(FILE *database_file, int page_no) {
  int current_page_offset = get_page_offset(page_no);

  unsigned char buffer[1];
  if (read_bytes(database_file, current_page_offset, 1, buffer) != 0) {
    perror("Failed to get the header size");
    return -1;
  }

  unsigned char byte = buffer[0];
  if (byte == TABLE_BTREE_LEAFCELL_HEADER ||
      byte == INDEX_BTREE_LEAFCELL_HEADER) {
    return 8;
  } else if (byte == TABLE_BTREE_INTERIORCELL_HEADER ||
             byte == INDEX_BTREE_INTERIORCELL_HEADER) {
    return 12;
  }

  fprintf(stderr, "Invalid header: %02X\n", byte);
  return -1;
}

unsigned short get_no_of_cells(FILE *database_file, int page_no) {
  unsigned char buffer[2];

  if (read_bytes(database_file, get_page_offset(page_no) + NO_OF_CELLS_OFFSET,
                 2, buffer) == 0) {
    return bytes_to_short(buffer);
  }

  perror("Failed to read the number of cells");
  return -1;
}

void get_cell_offsets(FILE *database_file, int page_no, short **output,
                      short no_of_cells) {
  int page_header_size = get_page_header_size(database_file, page_no);
  int page_offset = get_page_offset(page_no);

  *output = read_array_of_shorts(database_file, page_offset + page_header_size,
                                 no_of_cells);

  if (*output == NULL) {
    fprintf(stderr, "Failed to get the cell offsets\n");
  }
}

Column **get_columns_from_a_record(FILE *database_file, int offset,
                                   short *no_of_columns) {
  unsigned char buffer[1];
  if (read_bytes(database_file, offset + 2, 1, buffer) != 0) {
    perror("Failed to read the number of columns");
    return NULL;
  }

  *no_of_columns = (short)(buffer[0]) - 1;
  Column **columns = (Column **)malloc(*no_of_columns * sizeof(Column *));
  if (columns == NULL) {
    perror("Failed to allocate memory for columns array");
    return NULL;
  }

  for (short i = 0; i < *no_of_columns; ++i) {
    columns[i] = (Column *)malloc(sizeof(Column));
    if (columns[i] == NULL) {
      perror("Failed to allocate memory for column");
      free_columns(columns, i);
      return NULL;
    }

    int current_offset = offset + 3 + i;
    unsigned char buffer[1];

    if (read_bytes(database_file, current_offset, 1, buffer) != 0) {
      perror("Failed to read the column size");
      free_columns(columns, i + 1);
      return NULL;
    }

    get_column_size_and_dt(database_file, (short)buffer[0], columns[i]);
    if (strcmp(columns[i]->datatype, "UNKNOWN") == 0) {
      fprintf(stderr, "Unknown datatype for column %d\n", i + 1);
      free_columns(columns, i + 1);
      return NULL;
    }
  }

  return columns;
}

void get_column_size_and_dt(FILE *database_file, short size, Column *column) {
  switch (size) {
  case 0: {
    column->datatype = "NULL";
    column->size = 0;
    break;
  }
  case 1: {
    column->datatype = "INT8";
    column->size = 1;
    break;
  }
  case 2: {
    column->datatype = "INT16";
    column->size = 2;
    break;
  }
  case 3: {
    column->datatype = "INT24";
    column->size = 3;
    break;
  }
  case 4: {
    column->datatype = "INT32";
    column->size = 4;
    break;
  }
  case 5: {
    column->datatype = "INT48";
    column->size = 6;
    break;
  }
  case 6: {
    column->datatype = "INT64";
    column->size = 8;
    break;
  }
  case 7: {
    column->datatype = "FLOAT";
    column->size = 8;
    break;
  }
  case 8: {
    column->datatype = "0";
    column->size = 0;
    break;
  }
  case 9: {
    column->datatype = "1";
    column->size = 0;
    break;
  }
  default: {
    if (size >= 12 && size % 2 == 0) {
      column->datatype = "BLOB";
      column->size = (size - 12) / 2;
    } else if (size >= 13 && size % 2 != 0) {
      column->datatype = "TEXT";
      column->size = (size - 13) / 2;
    } else {
      column->datatype = "UNKNOWN";
    }
  }
  }
}

void free_columns(Column **columns, short no_of_columns) {
  if (columns != NULL) {
    for (int i = 0; i < no_of_columns; ++i) {
      if (columns[i] != NULL) {
        free(columns[i]);
        columns[i] = NULL;
      }
    }
    free(columns);
    columns = NULL;
  }
}
