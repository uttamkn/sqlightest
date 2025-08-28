#include "commands/dbinfo.h"
#include "commands/sql.h"
#include "commands/tables.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  FILE *file;
  const char *path;
} DatabaseConnection;

DatabaseConnection *open_database(const char *path) {
  DatabaseConnection *db = malloc(sizeof(DatabaseConnection));
  if (!db) {
    fprintf(stderr, "Memory allocation failed\n");
    return NULL;
  }

  db->file = fopen(path, "rb");
  if (!db->file) {
    fprintf(stderr, "Failed to open database file: %s\n", path);
    free(db);
    return NULL;
  }

  db->path = path;
  return db;
}

void close_database(DatabaseConnection *db) {
  if (db) {
    if (db->file) {
      fclose(db->file);
    }
    free(db);
  }
}

int execute_command(DatabaseConnection *db, const char *command) {
  if (!db || !db->file) {
    return -1;
  }

  if (strcmp(command, ".dbinfo") == 0) {
    return command_dbinfo(db->file);
  } else if (strcmp(command, ".tables") == 0) {
    return command_tables(db->file);
  } else {
    return command_query(db->file, command);
  }

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <database path> <command>\n", argv[0]);
    return 1;
  }

  DatabaseConnection *db = open_database(argv[1]);
  if (!db) {
    return 1;
  }

  int result = execute_command(db, argv[2]);

  close_database(db);

  return result;
}
