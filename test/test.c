int add() {
  return 42;
}

int div(int a, int b) {
  return a / b;
}

int inc(int a) {
  return a + 1;
}

int sum(int a, int b, int c, int d, int e, int f) {
  return a + b + c + d + e + f;
}

int test_unary_minus() {
  int a;
  a = 31;
  expect(190, -1, 0 - 1);
  expect(191, -30, -a + 1);
  return 0;
}

int main() {
  int a;
  int b;
  int c;
  int d;
  int e;
  int aa;

  test_unary_minus();
  /* comment */
  /** comment2 **/
  expect(41, 44, 46 - 7 + 5);
  expect(72, 16, 24 * 2 / 3);

  expect(71, 47, 5 + 6 * 7);

  expect(80, 40, (40));
  expect(91, 15, 5 * (9 - 6));

  expect(100, 10, a = 10);
  expect(101, 8, a = b = 8);
  expect(101, 8, a);
  expect(101, 8, b);
  expect(130, 29, aa = 28 + 1);

  a = 3;
  b = 5 * 6 - 8;
  expect(110, 14, a + b / 2);

  expect(120, 0, 19 == 22);
  expect(121, 1, 19 != 22);
  expect(122, 1, 16 == 33 - 17);
  expect(123, 1, 23 + 24 != 25);

  a = 3;
  if (1)
    a = 2;
  expect(140, 2, a);

  a = 3;
  if (0)
    a = 2;
  expect(141, 3, a);

  if (1)
    a = 2;
  else
    a = 3;
  expect(142, 2, a);

  if (0)
    a = 2;
  else
    a = 3;
  expect(143, 3, a);

  a = 1;
  if (1) {
    a = 3;
  }
  expect(144, 3, a);

  if (0) {
    a = 3;
  } else {
    a = 4;
  }
  expect(145, 4, a);

  a = 1;
  while (0)
    a = 2;
  expect(150, 1, a);

  a = 1;
  while (a == 1)
    a = 2;
  expect(151, 2, a);

  a = 1;
  while (1) {
    break;
    a = 2;
  }
  expect(152, 1, a);

  a = 1;
  while (a == 1) {
    a = a + 1;
    continue;
    a = 3;
  }
  expect(153, 2, a);

  expect(160, 42, add());
  expect(161, 4, div(12, 3));

  expect(170, 2, inc(1));
  a = 1;
  expect(171, 2, inc(a));
  expect(172, 1, a); /* a の値は書き換えられない */

  expect(180, 21, sum(1, 2, 3, 4, 5, 6));
  /*
  expect(,,);
  expect(162, 3, 5);
  */
  /* expect(999, 0, 1);  */

  return 0;
}
