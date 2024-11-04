#include "./include/dbinfo.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: ./your_program.sh <database path> <command>\n");
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

    printf("database page size: %u\n", getPageSize(database_file));
    printf("number of tables: %u\n", getNoOfTables(database_file));

    fclose(database_file);
  } else if (strcmp(command, ".tables") == 0) {
    FILE *database_file = fopen(database_file_path, "rb");
    if (!database_file) {
      fprintf(stderr, "Failed to open the database file\n");
      return 1;
    }

    /* printf("%s", getAllTables(database_file)); */

    fclose(database_file);
  } else {
    fprintf(stderr, "Unknown command %s\n", command);
    return 1;
  }

  return 0;
}
