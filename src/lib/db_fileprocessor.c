#include "../include/db_fileprocessor.h"
#include "../include/constants.h"
#include "../include/utils.h"

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

int get_page_header_size(FILE *database_file, int page_no) {
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

void get_cell_offsets(FILE *database_file, int page_no, int **output,
                      int no_of_cells) {
  int page_header_size = get_page_header_size(database_file, page_no);
  int page_offset = get_page_offset(page_no);

  *output = read_array_of_short(database_file, page_offset + page_header_size,
                                no_of_cells);

  if (*output == NULL) {
    fprintf(stderr, "Failed to get the cell offsets\n");
  }
}

void parse_record_data(Column *col[]);
