#ifndef UTIL_H
#define UTIL_H
// util
typedef struct {
  void **data;
  int capacity;
  int len;
} Vector;

Vector *new_vector();
void vec_push(Vector *, void *);

typedef struct {
  Vector *keys;
  Vector *vals;
} Map;

Map *new_map();
void map_put(Map *, char *, void *);
void *map_get(Map *, char *);

int *intdup(int);

_Noreturn void error(char *, ...);

// util_test
void run_utiltest();
#endif
