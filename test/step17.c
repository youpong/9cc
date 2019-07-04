int type_pointer() {
  int x;
  int *y;

  x = 3;
  y = &x;

  expect(1700, 3, *y);

  x = 4;

  expect(1701, 5, *y);
  
  return 0;
}

int main() {
  type_pointer();
  return 17;
}
