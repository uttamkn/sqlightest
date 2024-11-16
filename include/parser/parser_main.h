#ifndef PARSER_MAIN_H
#define PARSER_MAIN_H

#include <stdio.h>

typedef enum {
  TYPE_SELECT,
  TYPE_CREATE,
  TYPE_INSERT,
  TYPE_DELETE,
  TYPE_UPDATE
} QueryType;

typedef struct {
  QueryType type;
  char **tables;
  int num_tables;
  char **fields;
  int num_fields;
} Query;

typedef enum {
  stepStart,
  stepSelectFields,
  stepSelectFrom,
  stepSelectComma,
  stepSelectFromTable,
  stepEnd
} Step;

typedef struct {
  int i;           // Current position
  const char *sql; // The sql string that has to be parsed
  size_t sql_len;
  Query *query;
  Step step; // Current step
} Parser;

Query *parser_parse(Parser *p);
Parser *parser_new(const char *sql);
void parser_print(const Parser *p);
void parser_print_query(const Query *q);
void parser_free(Parser *p);

#endif // PARSER_MAIN_H
