#include "parser/tokeniser.h"
#include <stdio.h>

int tok_pop_space(Parser *p) {
  if (p->sql[p->i] != ' ') {
    fprintf(stderr, "Syntax error: expected a ' ' after the keyword");
    return -1;
  }
  while (p->sql[p->i] == ' ') {
    p->i++;
  }

  return 0;
}

int tok_compare_keyword(Parser *p, const char *keyword, int len) {
  if (!p || !keyword) {
    fprintf(stderr, "no keyword to compare");
    return -1;
  }

  if (p->i >= strlen(p->sql)) {
    fprintf(stderr, "End of the sql string reached");
    return -1;
  }

  return strncasecmp(p->sql + p->i, keyword, len);
}

int tok_peek_identifier(Parser *p) {
  if (p->sql[p->i] == ' ' || p->sql[p->i] == '\0') {
    fprintf(stderr, "Syntax error: expected an identifier");
    return -1;
  }

  // TODO: Implement peek_identifier function

  return 0;
}
