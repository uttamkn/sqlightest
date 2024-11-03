#ifndef DBINFO_H
#define DBINFO_H

#include <stdio.h>

unsigned int getNoOfPages(FILE *database_file);

unsigned short getPageSize(FILE *database_file);
unsigned int getNoOfTables(FILE *database_file);
unsigned short getNoOfCellsOfRootPage(FILE *database_file);

#endif // DBINFO_H
