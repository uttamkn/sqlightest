#include "./include/db_fileprocessor.h"
#include "./include/dbinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: ./run.sh <database path> <command>\n");
    return 1;
  }

  const char *database_file_path = argv[1];
  const char *command = argv[2];

  if (strcmp(command, ".dbinfo") == 0) {
    FILE *database_file = fopen(database_file_path, "rb");
    if (!database_file) {
      fprintf(stderr, "Failed to open the database file\n");
      return 1;
    }

    printf("database page size: %u\n", get_page_size(database_file));
    printf("number of tables: %u\n", get_no_of_tables(database_file));
    print_all_table_names(database_file);

    fclose(database_file);
  } else if (strcmp(command, ".tables") == 0) {
    FILE *database_file = fopen(database_file_path, "rb");
    if (!database_file) {
      fprintf(stderr, "Failed to open the database file\n");
      return 1;
    }

    short no_of_cells = get_no_of_cells(database_file, 1);
    short *cell_offsets;
    get_cell_offsets(database_file, 1, &cell_offsets, no_of_cells);

    short no_of_columns;
    Column **columns = get_columns_from_a_record(database_file, cell_offsets[0],
                                                 &no_of_columns);
    free_columns(columns, no_of_columns);
    free(cell_offsets);

    fclose(database_file);
  } else {
    fprintf(stderr, "Unknown command %s\n", command);
    return 1;
  }

  return 0;
}
