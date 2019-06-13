#include "9cc.h"
#include "util.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *yyin;
SYM_TAB *sym_tab = NULL;
bool ast_flg = false;
char **ARGV;

int main(int argc, char **argv) {

  if (argc >= 2 && strcmp(argv[1], "-test") == 0) {
    run_util_test();
    run_symbol_test();
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
  ARGV = argv;

  Vector *tokens = new_vector();
  Vector *code = new_vector();
  sym_tab = append_sym_tab(NULL);

  init();
  tokenize(tokens);
  parse(tokens, code);
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
