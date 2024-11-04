#include "../include/dbinfo.h"
#include "../include/offset.h"
#include "../include/utils.h"

unsigned short getPageSize(FILE *database_file) {
  unsigned char buffer[2];
  if (readBytes(database_file, PAGE_SIZE_OFFSET, 2, buffer) == 0) {
    return convertBytesToShort(buffer);
  }

  perror("Failed to read the page size");
  return -1;
}

unsigned short getNoOfCells(FILE *database_file, int page_no) {
  unsigned char buffer[2];

  int current_page_offset = HEADER_SIZE + DEFAULT_PAGE_SIZE * (page_no - 1);

  if (readBytes(database_file, current_page_offset + NO_OF_CELLS_OFFSET, 2,
                buffer) == 0) {
    return convertBytesToShort(buffer);
  }

  perror("Failed to read the number of cells");
  return -1;
}

unsigned int getNoOfTables(FILE *database_file) {
  // This is temporary (doesnt work if the tables are stored in more than one
  // page)
  return getNoOfCells(database_file, 1);
}
