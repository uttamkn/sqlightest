#include "commands/dbinfo.h"
#include "btree/btree_main.h"

int command_dbinfo(FILE *database_file) {
  int page_size = get_page_size(database_file);
  if (page_size == -1) {
    return -1;
  }

  int no_of_tables = get_no_of_tables(database_file);
  if (no_of_tables == -1) {
    return -1;
  }

  printf("database page size: %d\n", page_size);
  printf("number of tables: %d\n", no_of_tables);
  return 0;
}
