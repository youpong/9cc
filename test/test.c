
int main() {
  int a;
  int b;
  int c;
  int d;
  int e;
  int aa;

  /* comment */
  /** comment2 **/

  expect(31, 3, 1 + 2);
  expect(32, 4, 9 - 5);
  expect(40, 21, 5 + 20 - 4);
  expect(41, 44, 46 - 7 + 5);
  expect(50, 41, 12 + 34 - 5);

  expect(60, 18, 3 * 6);
  expect(70, 13, 39 / 3);
  expect(71, 47, 5 + 6 * 7);
  expect(72, 18, 24 / 4 * 3);
  expect(80, 40, (40));

  expect(90, 7, (41 + 43) / 12);
  expect(91, 15, 5 * (9 - 6));
  expect(92, 4, (3 + 5) / 2);
  expect(100, 10, c = 10);
  expect(101, 8, a = b = 8);

  a = b = 54;
  expect(102, 54, a);
  a = b = 55;
  expect(103, 55, b);
  d = 11;
  expect(104, 11, e = d);
  a = 36;
  b = 37;
  expect(105, 36, a);

  a = 3;
  b = 5 * 6 - 8;

  expect(110, 14, a + b / 2);

  expect(120, 0, 19 == 22);
  expect(121, 1, 19 != 22);
  expect(122, 1, 16 == 33 - 17);
  expect(123, 1, 23 + 24 != 25);
  expect(124, 0, 26 + 27 != 53);
  expect(130, 29, aa = 28 + 1);

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

  /*
  expect(,,);
  */
  /* expect(999, 0, 1);  */

  return 0;
}
