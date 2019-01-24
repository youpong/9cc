#include <stdio.h>
#include <stdlib.h>

int fibonacci(int);

int main(int argc, char **argv) {
  printf("%d\n", fibonacci(atoi(argv[1])));
  return 0;
}
