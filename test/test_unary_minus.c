int test_unary_minus() {
  int unary_minus;
  unary_minus = 31;

  expect(190, -1, -1 + 0);
  expect(190, 1, 0 - (-1));
  expect(191, -30, -unary_minus + 1);

  return 0;
}

int main() {
  test_unary_minus();
  return 100;
}
