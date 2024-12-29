#ifndef PAGE_H
#define PAGE_H

#include <stdio.h>

int get_page_size(FILE *database_file);
int get_page_offset(int page_no);
short get_page_header_size(FILE *database_file, int page_no);

#endif // PAGE_H
