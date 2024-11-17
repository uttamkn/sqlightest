#include "parser/tokeniser.h"
#include "memory.h"
#include <regex.h>
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
  if (!p) {
    fprintf(stderr, "Parser is NULL\n");
    return -1;
  }

  if (p->i >= p->sql_len || p->i + len > p->sql_len) {
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

// Returns 1 if the identifier is valid, 0 otherwise
int tok_validate_identifier(const char *identifier) {
  if (!identifier) {
    fprintf(stderr, "Identifier is NULL\n");
    return 0;
  }

  regex_t regex;
  int ret;

  ret = regcomp(&regex, "^[a-zA-Z_][a-zA-Z0-9_]*$", REG_EXTENDED);
  if (ret) {
    fprintf(stderr, "Failed to compile regex\n");
    return 0;
  }

  ret = regexec(&regex, identifier, 0, NULL, 0);
  regfree(&regex);

  if (!ret) {
    return 1; // match
  } else if (ret == REG_NOMATCH) {
    return 0; // no match
  } else {
    fprintf(stderr, "Regex matching failed\n");
    return 0;
  }
}

// Returns the length of the identifier
int tok_is_identifier(Parser *p) {
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

  char *identifier = mallox(len + 1);

  memcpy(identifier, p->sql + p->i, len);
  identifier[len] = '\0';

  if (tok_validate_identifier(identifier) == 0) {
    fprintf(stderr, "Syntax error: invalid identifier\n");
    free(identifier);
    return -1;
  }

  free(identifier);

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
  p->query->fields = (char **)reallox(
      p->query->fields, (p->query->num_fields + 1) * sizeof(char *));

  p->query->fields[p->query->num_fields] =
      (char *)mallox((len + 1) * sizeof(char));

  memcpy(p->query->fields[p->query->num_fields], p->sql + p->i, len);
  p->query->fields[p->query->num_fields][len] = '\0';

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
  p->query->tables = (char **)reallox(
      p->query->tables, (p->query->num_tables + 1) * sizeof(char *));

  p->query->tables[p->query->num_tables] =
      (char *)mallox((len + 1) * sizeof(char));

  memcpy(p->query->tables[p->query->num_tables], p->sql + p->i, len);
  p->query->tables[p->query->num_tables][len] = '\0';

  p->query->num_tables++;
  p->i += len;

  // WARN: if p->i == ' ' then the next step should be stepWhere
  p->step = stepEnd;

  return 0;
}
