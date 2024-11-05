#ifndef CELL_H
#define CELL_H

#include <stdio.h>

unsigned short get_no_of_cells(FILE *database_file, int page_no);
void get_cell_offsets(FILE *database_file, int page_no, short **output,
                      short no_of_cells);

#endif // CELL_H
