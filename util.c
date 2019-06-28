#include "util.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

Vector *new_vector() {
  Vector *vec = malloc(sizeof(Vector));
  vec->capacity = 16;
  vec->data = malloc(sizeof(void *) * vec->capacity);
  vec->len = 0;
  return vec;
}

void vec_push(Vector *vec, void *elem) {
  if (vec->capacity == vec->len) {
    vec->capacity *= 2;
    vec->data = realloc(vec->data, sizeof(void *) * vec->capacity);
  }
  vec->data[vec->len++] = elem;
}

void *vec_pop(Vector *vec) {
  return vec->data[--vec->len];
}

void *vec_last(Vector *vec) {
  return vec->data[vec->len - 1];
}

void *vec_at(Vector *vec, int index) {
  return vec->data[index];
}

Map *new_map() {
  Map *map = malloc(sizeof(Map));
  map->keys = new_vector();
  map->vals = new_vector();
  return map;
}

void map_put(Map *map, char *key, void *val) {
  vec_push(map->keys, key);
  vec_push(map->vals, val);
}

void *map_get(Map *map, char *key) {
  for (int i = map->keys->len - 1; i >= 0; i--) {
    if (strcmp((char *)vec_at(map->keys, i), key) == 0) {
      return vec_at(map->vals, i);
    }
  }
  return NULL;
}

Vector *map_keys(Map *map) {
  return map->keys;
}

int *intdup(int n) {
  int *num = malloc(sizeof(int));
  *num = n;
  return num;
}

noreturn void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

void expect(int line, int expected, int actual) {
  if (expected == actual)
    return;
  error("%d: %d expected, but got %d\n", line, expected, actual);
}

void expect_ptr(int line, void *expected, void *actual) {
  if (expected == actual)
    return;
  error("%d: %d expected, but got %d\n", line, expected, actual);
}
