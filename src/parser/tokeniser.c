#include "parser/tokeniser.h"
#include "memory.h"
#include <stdio.h>
#include <string.h>

static const char *keywords[] = {
    "SELECT",  "FROM",   "WHERE",   "INSERT",     "INTO", "VALUES",  "CREATE",
    "TABLE",   "UPDATE", "SET",     "DELETE",     "DROP", "ALTER",   "ADD",
    "PRIMARY", "KEY",    "FOREIGN", "REFERENCES", "JOIN", "LEFT",    "RIGHT",
    "OUTER",   "INNER",  "CROSS",   "UNION",      "ALL",  "ORDER",   "BY",
    "GROUP",   "HAVING", "LIMIT",   "OFFSET",     "ASC",  "DESC",    "NOT",
    "NULL",    "AND",    "OR",      "IN",         "LIKE", "BETWEEN", "IS",
    "TRUE",    "FALSE",  "UNKNOWN", ",",          ";",    "(",       ")",
    "<",       ">",      "<=",      ">=",         "=",    "!=",      "<>",
    "+",       "-",      "*",       "/",          NULL};

int tok_pop_space(Parser *p) {
  if (p->sql[p->i] != ' ') {
    fprintf(stderr, "Syntax error: expected a ' ' after the keyword\n");
    return -1;
  }
  while (p->sql[p->i] == ' ') {
    p->i++;
  }

  return 0;
}

int tok_pop_optional_space(Parser *p) {
  while (p->sql[p->i] == ' ') {
    p->i++;
  }

  return 0;
}

int tok_compare_keyword(Parser *p, const char *keyword, int len) {
  if (!p || !keyword) {
    fprintf(stderr, "no keyword to compare\n");
    return -1;
  }

  if (p->i >= p->sql_len) {
    fprintf(stderr, "End of the sql string reached\n");
    return -1;
  }

  return strncasecmp(p->sql + p->i, keyword, len);
}

int tok_is_asterisk(Parser *p) {
  if (p->i >= p->sql_len) {
    fprintf(stderr, "End of the sql string reached\n");
    return -1;
  }

  return p->sql[p->i] == '*';
}

// Returns the length of the identifier
int tok_is_identifier(Parser *p) {
  // TODO: validate the identifier (no special characters, etc)

  if (p->i >= p->sql_len) {
    fprintf(stderr, "End of the sql string reached\n");
    return -1;
  }

  if (p->sql[p->i] == ' ' || p->sql[p->i] == '\0') {
    fprintf(stderr, "Syntax error: expected an identifier\n");
    return -1;
  }

  int len = 0;
  while (p->sql[p->i + len] != ' ' && p->sql[p->i + len] != '\0' &&
         p->sql[p->i + len] != ',' && p->sql[p->i + len] != ';') {
    len++;
  }

  // check if the identifier is a keyword
  for (int i = 0; keywords[i] != NULL; i++) {
    if (strncasecmp(p->sql + p->i, keywords[i], len) == 0) {
      fprintf(stderr,
              "Syntax error: expected an identifier, but found a keyword\n");
      return -1;
    }
  }

  return len;
}

// Store the field name and move to the next step
int tok_peek_field(Parser *p, size_t len) {
  p->query->fields =
      reallox(p->query->fields, (p->query->num_fields + 1) * sizeof(char *));

  p->query->fields[p->query->num_fields] = mallox(len * sizeof(char));

  strncpy(p->query->fields[p->query->num_fields], p->sql + p->i, len);
  p->query->num_fields++;
  p->i += len;

  if (p->sql[p->i] == ',') {
    p->step = stepSelectComma;
    return 0;
  }

  if (tok_pop_space(p) != 0)
    return -1;

  p->step = stepSelectFrom;
  return 0;
}

// Store the table name and move to the next step
int tok_peek_table(Parser *p, size_t len) {
  p->query->tables =
      reallox(p->query->tables, (p->query->num_tables + 1) * sizeof(char *));

  p->query->tables[p->query->num_tables] = mallox(len * sizeof(char));

  strncpy(p->query->tables[p->query->num_tables], p->sql + p->i, len);
  p->query->num_tables++;
  p->i += len;

  // WARN: if p->i == ' ' then the next step should be stepWhere
  p->step = stepEnd;

  return 0;
}
