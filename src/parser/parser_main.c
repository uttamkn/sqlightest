#include "parser/parser_main.h"
#include "parser/tokeniser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Static functions (private to this file)
static int _step_start(Parser *p) {
  // TODO: Add other keywords (CREATE, INSERT etc)
  if (tok_compare_keyword(p, "SELECT", 6) == 0) {
    p->i += 6;
    p->query->type = TYPE_SELECT;
    p->step = stepSelectFields;
    if (tok_pop_space(p) != 0)
      return -1;
  } else {
    fprintf(stderr, "Syntax error: expected a 'SELECT' keyword others are not "
                    "implemented yet\n");
    return -1;
  }

  return 0;
}

Query *parser_parse(Parser *p) {
  if (!p) {
    perror("Parser is NULL");
    return NULL;
  }
  int n = strlen(p->sql);

  while (p->i < n) {
    switch (p->step) {
    case stepStart: {
      if (_step_start(p) != 0) {
        return NULL;
      }
      break;
    }
    case stepSelectFields: {
      // TODO: shift this to a separate tokeniser function (peek identifier)
      if (p->sql[p->i] == '*') {
        p->i += 1;
        p->query->fields = realloc(p->query->fields,
                                   (p->query->num_fields + 1) * sizeof(char *));
        p->query->fields[p->query->num_fields++] = strdup("*");
        p->step = stepSelectFrom;
        if (tok_pop_space(p) != 0)
          return NULL;
      } else {
        fprintf(stderr, "Syntax error: expected a '*' keyword other "
                        "keywords are not implemented yet\n");
        return NULL;
      }
      break;
    }
    case stepSelectComma: {
      // TODO: implement the logic for comma separated fields and tables

      break;
    }
    case stepSelectFrom: {
      if (tok_compare_keyword(p, "FROM", 4) == 0) {
        p->i += 4;
        p->step = stepSelectFromTable;
        if (tok_pop_space(p) != 0)
          return NULL;
      } else {
        fprintf(stderr, "Syntax error: expected a 'FROM' keyword\n");
        return NULL;
      }
      break;
    }
    case stepSelectFromTable: {
      // TODO: shift this to a separate tokeniser function (peek identifier)
      p->query->tables = realloc(p->query->tables,
                                 (p->query->num_tables + 1) * sizeof(char *));
      p->query->tables[p->query->num_tables] = malloc(n - p->i + 1);
      strcpy(p->query->tables[p->query->num_tables], p->sql + p->i);
      p->i += n - p->i;
      p->query->num_tables++;
      break;
    }
    }
  }

  return p->query;
}

Query *parser_new_query() {
  Query *q = (Query *)malloc(sizeof(Query));
  if (q == NULL) {
    free(q);
    perror("malloc failed while creating new Query struct");
    return NULL;
  }
  q->fields = NULL;
  q->tables = NULL;
  q->num_fields = 0;
  q->num_tables = 0;

  return q;
}

Parser *parser_new(const char *sql) {
  Parser *p = malloc(sizeof(Parser));
  if (p == NULL) {
    perror("malloc failed while creating new parser");
    return NULL;
  }
  p->i = 0;
  p->step = stepStart;
  p->query = parser_new_query();
  p->sql = sql;
  return p;
}

void parser_print_query(const Query *q) {
  printf("Query type: %u\n", q->type);

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

void parser_print(const Parser *p) {
  printf("SQL: %s\n", p->sql);
  printf("i: %d\n", p->i);
  printf("Step: %u\n", p->step);
  parser_print_query(p->query);
}

void parser_free_query(Query *q) {
  if (q != NULL) {
    if (q->tables != NULL) {
      for (int i = 0; i < q->num_tables; i++) {
        free(q->tables[i]);
      }
      free(q->tables);
    }

    if (q->fields != NULL) {
      for (int i = 0; i < q->num_fields; i++) {
        free(q->fields[i]);
      }
      free(q->fields);
    }

    free(q);
  }
}

void parser_free(Parser *p) {
  if (p != NULL) {
    parser_free_query(p->query);
    free(p);
  }
}
