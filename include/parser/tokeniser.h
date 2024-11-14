#ifndef TOKENISER_H
#define TOKENISER_H

#include "parser_main.h"
#include <stdio.h>
#include <string.h>

int tok_pop_space(Parser *p);
int tok_compare_keyword(Parser *p, const char *keyword, int len);
int tok_peek_identifier(Parser *p);

#endif // TOKENISER_H
