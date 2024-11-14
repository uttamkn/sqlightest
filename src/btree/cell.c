#include "btree/cell.h"
#include "btree/page.h"
#include "constants.h"
#include "conversion.h"
#include "fileprocessor.h"

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
    perror("Failed to get the cell offsets\n");
  }
}
