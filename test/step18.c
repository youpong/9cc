/*
#include "util.h"
void alloc4(int **, int, int, int, int);
*/
int step18() {
  int *p;
  int *q;
  
  alloc4(&p, 1, 2, 4, 8);

  q = p + 0;
  expect(1800, 1, *q);

  q = q + 2;
  expect(1801, 4, *q);

  q = q - 1;
  expect(1802, 2, *q);

  expect(1803, 1, *(p));
  
  expect(1804, 8, *(p + 3));  

  return 0;
}

int main() {
  step18();
  return 18;
}
