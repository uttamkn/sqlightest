#include "../include/dbinfo.h"
#include "../include/utils.h"

unsigned short getPageSize(FILE *database_file) {
  unsigned char buffer[2];
  if (readBytes(database_file, 16, 2, buffer) == 0) {
    return convertBytesToShort(buffer);
  }

  perror("Failed to read the page size");
  return -1;
}

unsigned short getNoOfCellsOfRootPage(FILE *database_file) {
  unsigned char buffer[2];
  if (readBytes(database_file, 103, 2, buffer) == 0) {
    return convertBytesToShort(buffer);
  }

  perror("Failed to read the number of cells");
  return -1;
}

unsigned int _getNoOfPages(FILE *database_file) {
  unsigned char buffer[4];
  if (readBytes(database_file, 28, 4, buffer) == 0) {
    return convertBytesToInt(buffer);
  }

  perror("Failed to read the number of pages");
  return -1;
}

unsigned int getNoOfTables(FILE *database_file) {
  // This is temporary (doesnt work if the tables are stored in more than one
  // page)
  return getNoOfCellsOfRootPage(database_file);
}
