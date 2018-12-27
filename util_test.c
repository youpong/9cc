#include "9cc.h"
#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void expect(int line, int expected, int actual) {
  if (expected == actual)
    return;
  error("%d: %d expected, but got %d\n", line, expected, actual);
}

static void expect_ptr(int line, void *expected, void *actual) {
  if (expected == actual)
    return;
  error("%d: %d expected, but got %d\n", line, expected, actual);
}

static void test_vector() {
  Vector *vec = new_vector();

  expect(__LINE__, 0, vec->len);

  for (int i = 0; i < 100; i++) {
    vec_push(vec, intdup(i));
  }

  expect(__LINE__, 100, vec->len);
  expect(__LINE__, 0, *(int *)vec->data[0]);
  expect(__LINE__, 50, *(int *)vec->data[50]);
  expect(__LINE__, 99, *(int *)vec->data[99]);
  expect(__LINE__, 99, *(int *)vec_last(vec));
}

static void test_map() {
  Map *map = new_map();

  expect_ptr(__LINE__, NULL, map_get(map, "foo"));

  map_put(map, "foo", intdup(2));
  expect(__LINE__, 2, *(int *)map_get(map, "foo"));

  map_put(map, "bar", intdup(4));
  expect(__LINE__, 4, *(int *)map_get(map, "bar"));

  map_put(map, "foo", intdup(6));
  expect(__LINE__, 6, *(int *)map_get(map, "foo"));
}

void run_utiltest() {
  test_vector();
  test_map();
  printf("OK\n");
}
