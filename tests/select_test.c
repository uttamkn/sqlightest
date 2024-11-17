#include "parser/parser_main.h"
#include <stdio.h>
#include <string.h>

int validate_query(Query *q, QueryType expected_type,
                   const char **expected_fields, int num_fields,
                   const char *expected_table) {
  if (!q) {
    fprintf(stderr, "Test failed: Query is NULL\n");
    return -1;
  }

  if (q->type != expected_type) {
    fprintf(stderr, "Test failed: Expected query type %d, got %d\n",
            expected_type, q->type);
    return -2;
  }

  if (q->num_fields != num_fields) {
    fprintf(stderr, "Test failed: Expected %d fields, got %d\n", num_fields,
            q->num_fields);
    return -3;
  }

  for (int i = 0; i < num_fields; i++) {
    if (strcmp(q->fields[i], expected_fields[i]) != 0) {
      fprintf(stderr,
              "Test failed: Field mismatch at %d, expected '%s', got '%s'\n", i,
              expected_fields[i], q->fields[i]);
      return -4;
    }
  }

  if (q->num_tables != 1 || strcmp(q->tables[0], expected_table) != 0) {
    fprintf(stderr, "Test failed: Expected table '%s', got '%s'\n",
            expected_table, q->num_tables ? q->tables[0] : "(none)");
    return -5;
  }

  printf("Test passed\n");
  return 0;
}

int test_command_query_valid() {
  const char *query = "SELECT name, age FROM users";

  Parser *p = parser_new(query);
  Query *q = parser_parse(p);

  const char *expected_fields[] = {"name", "age"};
  int result = validate_query(q, TYPE_SELECT, expected_fields, 2, "users");

  parser_free(p);
  return result;
}

int test_command_query_no_fields() {
  const char *query = "SELECT FROM users";

  Parser *p = parser_new(query);
  Query *q = parser_parse(p);

  if (q) {
    fprintf(stderr, "Test failed: Invalid syntax should return NULL Query\n");
    parser_free(p);
    return -1;
  } else {
    printf("Test passed: Invalid syntax handled correctly\n");
  }

  parser_free(p);
  return 0;
}

int test_command_query_no_table() {
  const char *query = "SELECT name, age";

  Parser *p = parser_new(query);
  Query *q = parser_parse(p);

  if (q) {
    fprintf(stderr, "Test failed: Invalid syntax should return NULL Query\n");
    parser_free(p);
    return -1;
  } else {
    printf("Test passed: Invalid syntax handled correctly\n");
  }

  parser_free(p);
  return 0;
}

int test_command_query_invalid_syntax() {
  const char *query = "SEL name, age FROM users";

  Parser *p = parser_new(query);
  Query *q = parser_parse(p);

  if (q) {
    fprintf(stderr, "Test failed: Invalid syntax should return NULL Query\n");
    parser_free(p);
    return -1;
  } else {
    printf("Test passed: Invalid syntax handled correctly\n");
  }

  parser_free(p);
  return 0;
}

// WARN: this test is failing
int test_command_query_extra_whitespace() {
  const char *query = "SELECT   name ,   age   FROM    users   ";

  Parser *p = parser_new(query);
  Query *q = parser_parse(p);

  const char *expected_fields[] = {"name", "age"};
  int result = validate_query(q, TYPE_SELECT, expected_fields, 2, "users");

  parser_free(p);
  return result;
}

int main() {
  int test_result = 0;

  test_result |= test_command_query_valid();
  test_result |= test_command_query_no_fields();
  test_result |= test_command_query_no_table();
  test_result |= test_command_query_invalid_syntax();
  test_result |= test_command_query_extra_whitespace();

  if (test_result == 0) {
    printf("All tests passed\n");
  } else {
    printf("Some tests failed\n");
  }

  return test_result;
}
