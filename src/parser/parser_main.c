#include "parser/parser_main.h"
#include <stdio.h>
#include <stdlib.h>

Query *parser_parse(Parser *p) {
  if (!p) {
    perror("Parser is NULL");
    return NULL;
  }
  // TODO: Parse the query
  return p->query;
}

Parser *parser_new(const char *sql) {
  Parser *p = malloc(sizeof(Parser));
  if (p == NULL) {
    perror("malloc failed while creating new parser");
    return NULL;
  }
  p->i = 0;
  p->step = "start";
  p->query = malloc(sizeof(Query));
  if (p->query == NULL) {
    free(p);
    perror("malloc failed while creating new parser");
    return NULL;
  }
  p->sql = sql;
  return p;
}

void parser_print(const Parser *p) {
  printf("SQL: %s\n", p->sql);
  printf("i: %d\n", p->i);
  printf("Step: %s\n", p->step);
  parser_print_query(p->query);
}

void parser_print_query(const Query *q) {
  printf("Query type: %s\n", q->type);

  if (q->tables == NULL) {
    printf("Tables: NULL\n");
  } else {
    printf("Tables: ");
    for (int i = 0; q->tables[i] != NULL; i++) {
      printf("%s ", q->tables[i]);
    }
    printf("\n");
  }

  if (q->fields == NULL) {
    printf("Fields: NULL\n");
  } else {
    printf("Fields: ");
    for (int i = 0; q->fields[i] != NULL; i++) {
      printf("%s ", q->fields[i]);
    }
    printf("\n");
  }
}

void parser_free(Parser *p) {
  if (p) {
    if (p->query->tables != NULL) {
      for (int i = 0; p->query->tables[i] != NULL; i++) {
        free(p->query->tables[i]);
      }
      free(p->query->tables);
    }

    if (p->query->fields != NULL) {
      for (int i = 0; p->query->fields[i] != NULL; i++) {
        free(p->query->fields[i]);
      }
      free(p->query->fields);
    }

    free(p->query);
    free(p);
  }
}
