#include "commands/sql.h"
#include "parser/parser_main.h"

int command_query(FILE *database_file, const char *query) {
  Parser *p = parser_new(query);

  Query *q = parser_parse(p);

  if (!q) {
    parser_free(p);
    return -1;
  }

  parser_print_query(q);

  parser_free(p);
  return 0;
}
