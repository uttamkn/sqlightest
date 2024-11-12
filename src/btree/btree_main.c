#include "btree/btree_main.h"

unsigned int get_no_of_tables(FILE *database_file) {
  // WARN: This function is not implemented properly yet (most of the time the
  // number of cells will be equal to the number of tables so number of cells is
  // being returned for now)
  return get_no_of_cells(database_file, 1);
}
