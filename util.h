#pragma once
/* util.c */
typedef struct {
  void **data;
  int capacity;
  int len;
} Vector;

Vector *new_vector();
void vec_push(Vector *, void *);
void *vec_pop(Vector *);
void *vec_last(Vector *);

typedef struct {
  Vector *keys;
  Vector *vals;
} Map;

Map *new_map();
void map_put(Map *, char *, void *);
void *map_get(Map *, char *);

int *intdup(int);

_Noreturn void error(char *, ...);

/* util_test.c */
void run_utiltest();
