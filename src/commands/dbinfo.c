#include "commands/dbinfo.h"
#include "btree/btree_main.h"

void command_dbinfo(FILE *database_file) {
  printf("database page size: %u\n", get_page_size(database_file));
  printf("number of tables: %u\n", get_no_of_tables(database_file));
}
