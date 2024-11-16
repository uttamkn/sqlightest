#ifndef TOKENISER_H
#define TOKENISER_H

#include "parser_main.h"

int tok_pop_space(Parser *p);
int tok_pop_optional_space(Parser *p);
int tok_compare_keyword(Parser *p, const char *keyword, int len);
int tok_is_asterisk(Parser *p);
int tok_is_identifier(Parser *p);
int tok_peek_field(Parser *p, size_t len);
int tok_peek_table(Parser *p, size_t len);

#endif // TOKENISER_H
