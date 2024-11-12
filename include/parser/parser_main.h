#ifndef PARSER_MAIN_H
#define PARSER_MAIN_H

typedef struct {
  char *type; // Type of the query (SELECT, CREATE, INSERT etc)
  char **tables;
  char **fields;
} Query;

typedef struct {
  int i;           // Current position
  const char *sql; // The sql string that has to be parsed
  Query *query;
  char *step; // Current step
} Parser;

Query *parser_parse(Parser *p);
Parser *parser_new(const char *sql);
void parser_print(const Parser *p);
void parser_print_query(const Query *q);
void parser_free(Parser *p);

#endif // PARSER_MAIN_H
