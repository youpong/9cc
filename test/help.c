#include <stdlib.h>

void alloc4(int **p, int p0, int p1, int p2, int p3) {
  int *ptr = malloc(4 * sizeof(int));
  *p = ptr;

  *ptr++ = p0;
  *ptr++ = p1;
  *ptr++ = p2;
  *ptr++ = p3;
}
