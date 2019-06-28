#include "9cc.h"
#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void test_vector() {
  Vector *vec = new_vector();

  expect(__LINE__, 0, vec->len);

  for (int i = 0; i < 100; i++) {
    vec_push(vec, intdup(i));
  }

  expect(__LINE__, 100, vec->len);
  expect(__LINE__, 0, *(int *)vec_at(vec, 0));
  expect(__LINE__, 50, *(int *)vec_at(vec, 50));
  expect(__LINE__, 99, *(int *)vec_at(vec, 99));
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

void run_util_test() {
  test_vector();
  test_map();
}
