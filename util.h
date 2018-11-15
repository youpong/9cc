#ifndef UTIL_H
#define UTIL_H
// util
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

Map *new_map();

void map_put(Map *, char *, void *);

void *map_get(Map *, char *);
_Noreturn void error(char *, ...);

// util_test
void run_utiltest();
#endif
