#include "9cc.h"
#include "util.h"
#include <stdio.h>

static void test_query_var();

void run_symbol_test() {
  test_query_var();
  printf("OK\n");
}

static void test_query_var() {
  //  int a;
  expect_ptr(__LINE__, NULL, query_var("a"));
  sym_tab = append_sym_tab(NULL);
  expect_ptr(__LINE__, NULL, query_var("a"));
  sym_tab = append_sym_tab("main");
  expect_ptr(__LINE__, NULL, query_var("a"));
}
