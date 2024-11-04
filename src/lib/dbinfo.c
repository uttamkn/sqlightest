#include "../include/dbinfo.h"
#include "../include/constants.h"
#include "../include/db_fileprocessor.h"
#include "../include/utils.h"

unsigned short getPageSize(FILE *database_file) {
  unsigned char buffer[2];
  if (readBytes(database_file, PAGE_SIZE_OFFSET, 2, buffer) == 0) {
    return convertBytesToShort(buffer);
  }

  perror("Failed to read the page size");
  return -1;
}

unsigned int getNoOfTables(FILE *database_file) {
  // This is temporary (doesnt work if the tables are stored in more than one
  // page)
  return getNoOfCells(database_file, 1);
}
