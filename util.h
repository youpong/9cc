#pragma once
#include <stdnoreturn.h>

/* util.c */
typedef struct {
  void **data;
  int capacity;
  int len;
} Vector;

typedef struct {
  Vector *keys;
  Vector *vals;
} Map;

Vector *new_vector();
void vec_push(Vector *, void *);
void *vec_pop(Vector *);
void *vec_last(Vector *);
void *vec_at(Vector *, int);

Map *new_map();
void map_put(Map *, char *, void *);
void *map_get(Map *, char *);
Vector *map_keys(Map *);

int *intdup(int);

noreturn void error(char *, ...);

void expect(int, int, int);
void expect_ptr(int, void *, void *);

/* util_test.c */
void run_util_test();
