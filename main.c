#include "9cc.h"
#include "util.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *yyin;
Vector *tokens;
int pos = 0;
Vector *code;
SYM_TAB *sym_tab = NULL;
bool ast_flg = false;
char **ARGV;

int main(int argc, char **argv) {
  tokens = new_vector();
  code = new_vector();
  sym_tab = append_sym_tab(NULL);

  if (argc >= 2 && strcmp(argv[1], "-test") == 0) {
    run_utiltest();
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

  init();
  tokenize();
  parse();

  if (ast_flg == true) {
    printf("graph graphname {\n");
    for (int i = 0; i < code->len; i++)
      p_tree((Node *)code->data[i]);
    printf("}\n");
    return EXIT_SUCCESS;
  }

  // print preamble.
  printf(".intel_syntax noprefix\n");

  // print global function names.
  printf(".global ");
  char *delim = "";
  Vector *keys = map_keys(sym_tab->children);
  for (int i = 0; i < keys->len; i++) {
    printf("%s%s", delim, (char *)vec_at(keys, i));
    delim = ", ";
  }
  printf("\n");

  // print each node.
  for (int i = 0; i < code->len; i++) {
    Node *node = (Node *)code->data[i];
    gen(node);

    // 式の評価結果としてスタックに一つの値が残っているはずなので
    // スタックが溢れないようにポップしておく
    printf("\tpop rax\n");
  }

  return EXIT_SUCCESS;
}
