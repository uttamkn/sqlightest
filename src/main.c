#include "commands/dbinfo.h"
#include "commands/sql.h"
#include "commands/tables.h"
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

    command_dbinfo(database_file);

    fclose(database_file);
  } else if (strcmp(command, ".tables") == 0) {
    FILE *database_file = fopen(database_file_path, "rb");
    if (!database_file) {
      fprintf(stderr, "Failed to open the database file\n");
      return 1;
    }

    command_tables(database_file);

    fclose(database_file);
  } else {
    FILE *database_file = fopen(database_file_path, "rb");
    if (!database_file) {
      fprintf(stderr, "Failed to open the database file\n");
      return 1;
    }

    command_query(database_file, command);

    fclose(database_file);
  }

  return 0;
}
