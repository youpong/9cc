#include "9cc.h"
#include "util.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SYM_TAB *sym_tab = NULL;

int main(int argc, char **argv) {
  bool ast_flg = false;

  // process options
  if (argc >= 2 && strcmp(argv[1], "-test") == 0) {
    run_util_test();
    run_symbol_test();
    run_pointer_test();
    printf("OK\n");
    return 0;
  }
  if (argc >= 2 && strcmp(argv[1], "-ast") == 0) {
    ast_flg = true;
    argc--;
    argv++;
  }
  if (argc != 2) {
    fprintf(stderr, "引数の数が正しくありません\n");
    fprintf(stderr, "%s [-ast] [-e program]\n", argv[0]);
    fprintf(stderr, "%s [-ast] [programfile]\n", argv[0]);
    fprintf(stderr, "%s -test\n", argv[0]);
    return EXIT_FAILURE;
  }

  init();
  Vector *tokens = tokenize(argv[1]);
  Vector *code = parse(tokens);
  sema(code);

  if (ast_flg == true) {
    printf("graph graphname {\n");
    for (int i = 0; i < code->len; i++)
      p_tree((Node *)vec_at(code, i));
    printf("}\n");
    return EXIT_SUCCESS;
  }

  gen_x86(code);

  return EXIT_SUCCESS;
}
