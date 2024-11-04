#ifndef DBINFO_H
#define DBINFO_H

#include <stdio.h>

unsigned short getPageSize(FILE *database_file);

unsigned int getNoOfTables(FILE *database_file);

#endif // DBINFO_H
