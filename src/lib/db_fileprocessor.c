#include "../include/db_fileprocessor.h"
#include "../include/constants.h"
#include "../include/utils.h"

int getPageOffset(int page_no) {
  int current_page_offset = 100; // offset for page 1
  if (page_no != 1) {
    current_page_offset = DEFAULT_PAGE_SIZE * (page_no - 1);
  }

  return current_page_offset;
}

int getPageHeaderSize(FILE *database_file, int page_no) {
  int current_page_offset = getPageOffset(page_no);

  unsigned char buffer[1];
  if (readBytes(database_file, current_page_offset, 1, buffer) == 0) {
    unsigned byte = buffer[0];
    if (byte == 16 || byte == 10) {
      return 8;
    } else if (byte == 5 || byte == 2) {
      return 12;
    }

    perror("Invalid header size");
    return -1;
  }

  perror("Failed to get the header size");
  return -1;
}

unsigned short getNoOfCells(FILE *database_file, int page_no) {
  unsigned char buffer[2];

  if (readBytes(database_file, getPageOffset(page_no) + NO_OF_CELLS_OFFSET, 2,
                buffer) == 0) {
    return convertBytesToShort(buffer);
  }

  perror("Failed to read the number of cells");
  return -1;
}

int *getCellOffsets(FILE *database_file, int page_no) {
  int no_of_cells = getNoOfCells(database_file, page_no);
}
