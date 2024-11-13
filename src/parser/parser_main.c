#include "parser/parser_main.h"
#include "parser/tokeniser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Query *parser_parse(Parser *p) {
  if (!p) {
    perror("Parser is NULL");
    return NULL;
  }
  int n = strlen(p->sql);

  // TODO: move all strncasecmp's into a single function in tokeniser
  while (p->i < n) {
    switch (p->step) {
    case stepStart: {
      // TODO: move this (stepStart) into a seperate static function and add
      // other keywords
      if (strncasecmp(p->sql + p->i, "SELECT", 6) == 0) {
        p->i += 6;
        // TODO: maybe you should make an enum for this instead
        p->query->type = "SELECT";
        p->step = stepSelectFields;
        if (tokeniser_pop_space(p) != 0)
          return NULL;
      } else {
        fprintf(stderr, "Syntax error: expected a 'SELECT' keyword other "
                        "keywords are not implemented yet");
        return NULL;
      }
      break;
    }
    case stepSelectFields: {
      // TODO: shift this to a separate tokeniser function
      if (p->sql[p->i] == '*') {
        p->i += 1;
        p->query->fields = malloc(sizeof(char *) * 2);
        p->query->fields[0] = "*";
        p->query->fields[1] = NULL;
        p->step = stepSelectFrom;
        if (tokeniser_pop_space(p) != 0)
          return NULL;
      } else {
        fprintf(stderr, "Syntax error: expected a '*' keyword other "
                        "keywords are not implemented yet");
        return NULL;
      }
      break;
    }
    case stepSelectComma: {
      // TODO: implement the logic for comma separated fields and tables

      break;
    }
    case stepSelectFrom: {
      if (strncasecmp(p->sql + p->i, "FROM", 4) == 0) {
        p->i += 4;
        p->step = stepSelectFromTable;
        if (tokeniser_pop_space(p) != 0)
          return NULL;
      } else {
        fprintf(stderr, "Syntax error: expected a 'FROM' keyword");
        return NULL;
      }
      break;
    }
    case stepSelectFromTable: {
      // TODO: memory management should be done properly (move this into a
      // function in tokeniser)
      p->query->tables = malloc(sizeof(char *) * 2);
      p->query->tables[0] = malloc(n - p->i + 1);
      strcpy(p->query->tables[0], p->sql + p->i);
      p->query->tables[1] = NULL;
      p->i += n - p->i;
      break;
    }
    }
  }

  return p->query;
}

Parser *parser_new(const char *sql) {
  Parser *p = malloc(sizeof(Parser));
  if (p == NULL) {
    perror("malloc failed while creating new parser");
    return NULL;
  }
  p->i = 0;
  p->step = stepStart;
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
  printf("Step: %u\n", p->step);
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
  // TODO: the tables and fields should be freed properly
  if (p != NULL) {
    if (p->query->tables != NULL) {
      free(p->query->tables);
    }

    if (p->query->fields != NULL) {
      free(p->query->fields);
    }

    free(p->query);
    free(p);
  }
}
