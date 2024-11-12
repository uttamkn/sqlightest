#include "btree/page.h"
#include "constants.h"
#include "conversion.h"
#include "fileprocessor.h"

unsigned short get_page_size(FILE *database_file) {
  unsigned char buffer[2];
  if (read_bytes(database_file, PAGE_SIZE_OFFSET, 2, buffer) == 0) {
    return bytes_to_short(buffer);
  }

  perror("Failed to read the page size");
  return -1;
}

int get_page_offset(int page_no) {
  int current_page_offset = 100; // offset for page 1
  if (page_no != 1) {
    current_page_offset = DEFAULT_PAGE_SIZE * (page_no - 1);
  }

  return current_page_offset;
}

short get_page_header_size(FILE *database_file, int page_no) {
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
