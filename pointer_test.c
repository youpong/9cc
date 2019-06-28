#include "util.h"

static void test_sizeof_pointer() {
  expect(__LINE__, 1, 1);
  expect(__LINE__, sizeof(void *), 8);
}

void run_pointer_test() {
  test_sizeof_pointer();
}
