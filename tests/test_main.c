#include "test_suites.h"
#include <stdio.h>

typedef int (*TestSuiteFunction)(void);

typedef struct {
  const char *name;
  TestSuiteFunction run;
} TestSuite;

int main(void) {
  const TestSuite suites[] = {
      {"select", run_select_tests},
  };
  const int suite_count = sizeof(suites) / sizeof(suites[0]);
  int failed_suites = 0;

  for (int i = 0; i < suite_count; i++) {
    printf("Running %s tests...\n", suites[i].name);

    if (suites[i].run() != 0) {
      fprintf(stderr, "%s tests failed\n", suites[i].name);
      failed_suites++;
    } else {
      printf("%s tests passed\n", suites[i].name);
    }
  }

  if (failed_suites == 0) {
    printf("All test suites passed\n");
    return 0;
  }

  fprintf(stderr, "%d test suite(s) failed\n", failed_suites);
  return 1;
}
