#include "parser/tokeniser.h"

int tokeniser_pop_space(Parser *p) {
  if (p->sql[p->i] != ' ') {
    fprintf(stderr, "Syntax error: expected a ' ' after the keyword");
    return -1;
  }
  while (p->sql[p->i] == ' ') {
    p->i++;
  }

  return 0;
}

int tokeniser_peek_identifier(Parser *p) {
  if (p->sql[p->i] == ' ' || p->sql[p->i] == '\0') {
    fprintf(stderr, "Syntax error: expected an identifier");
    return -1;
  }

  return 0;
}
