#include "commands/sql.h"
#include "parser/parser_main.h"

void command_query(FILE *database_file, const char *query) {
  Parser *p = parser_new(query);

  Query *q = parser_parse(p);

  parser_print(p);

  parser_free(p);
}
