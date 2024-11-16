#include "parser/parser_main.h"
#include "memory.h"
#include "parser/tokeniser.h"
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

  size_t field_len = 0;

  while (p->i < p->sql_len) {
    switch (p->step) {
    case stepStart: {
      if (_step_start(p) != 0) {
        return NULL;
      }
      break;
    }
    case stepSelectFields: {
      if ((field_len = tok_is_asterisk(p)) == 1) {
        if (tok_peek_field(p, field_len) != 0) {
          return NULL;
        }
      } else if ((field_len = tok_is_identifier(p)) != -1) {
        if (tok_peek_field(p, field_len) != 0) {
          return NULL;
        }
      } else {
        fprintf(stderr, "Syntax error: expected an identifier\n");
        return NULL;
      }
      break;
    }
    case stepSelectComma: {
      p->i++;
      tok_pop_optional_space(p);
      p->step = stepSelectFields;

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
      int table_len = tok_is_identifier(p);
      if (table_len == -1)
        return NULL;

      if (tok_peek_table(p, table_len) != 0) {
        return NULL;
      }

      break;
    }
    case stepEnd: {
      p->i = p->sql_len;
      break;
    }
    }
  }

  return p->query;
}

Query *parser_new_query() {
  Query *q = (Query *)mallox(sizeof(Query));
  q->fields = NULL;
  q->tables = NULL;
  q->num_fields = 0;
  q->num_tables = 0;

  return q;
}

Parser *parser_new(const char *sql) {
  int n = strlen(sql);

  if (!sql || n == 0) {
    fprintf(stderr, "SQL string is NULL\n");
    exit(1);
  }

  Parser *p = mallox(sizeof(Parser));
  p->i = 0;
  p->step = stepStart;
  p->query = parser_new_query();
  p->sql = sql;
  p->sql_len = n;
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
