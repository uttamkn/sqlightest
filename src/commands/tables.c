#include "btree/cell.h"
#include "btree/column.h"
#include "fileprocessor.h"
#include <stdlib.h>

void _print_all_table_names(FILE *database_file) {
  short no_of_cells = get_no_of_cells(database_file, 1);
  short *cell_offsets;
  get_cell_offsets(database_file, 1, &cell_offsets, no_of_cells);
  if (cell_offsets == NULL) {
    perror("Failed to get cell offsets");
    return;
  }

  for (short i = 0; i < no_of_cells; ++i) {
    int cell_offset = cell_offsets[i];
    short no_of_columns;
    Column **columns =
        get_columns_from_a_record(database_file, cell_offset, &no_of_columns);
    if (columns == NULL) {
      perror("Failed to get columns from a record");
      return;
    }

    unsigned char table_name[columns[1]->size];

    int cell_header_size = 2 + no_of_columns + 1;
    if (read_bytes(database_file,
                   cell_offset + cell_header_size + columns[0]->size,
                   columns[1]->size, table_name) != 0) {
      free_columns(columns, no_of_columns);
      perror("Failed to read the table name");
      return;
    }

    printf("%.*s ", columns[1]->size, table_name);
    free_columns(columns, no_of_columns);
  }

  free(cell_offsets);
}

void command_tables(FILE *database_file) {
  _print_all_table_names(database_file);
}
