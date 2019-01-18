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
Map *var_tab;
int var_cnt = 0;
bool cmdln_flg = false;
bool ast_flg = false;
char **ARGV;

int main(int argc, char **argv) {
  tokens = new_vector();
  code = new_vector();
  var_tab = new_map();

  if (argc >= 2 && strcmp(argv[1], "-test") == 0) {
    run_utiltest();
    return 0;
  }
  if (argc >= 2 && strcmp(argv[1], "-ast") == 0) {
    ast_flg = true;
    argc--;
    argv++;
  }
  if (argc >= 2 && strcmp(argv[1], "-e") == 0) {
    cmdln_flg = true;
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

  printf(".intel_syntax noprefix\n");
  if (cmdln_flg != true) {
    // function name
    char *delim = "";
    printf(".global ");
    for (int i = 0; i < code->len; i++) {
      Node *node = (Node *)code->data[i];
      if (node->ty == ND_FUNC_DEF) {
	printf("%s%s", delim, node->name);
	delim = ", ";
      }
    }
    printf("\n");
  } else {
    printf(".global main\n");
    printf("main:\n");
    printf("\tpush rbp\n");
    printf("\tmov rbp, rsp\n");
    printf("\tsub rsp, %d\n", var_cnt * 8);
  }

  for (int i = 0; i < code->len; i++) {
    Node *node = (Node *)code->data[i];
    gen(node);

    // 式の評価結果としてスタックに一つの値が残っているはずなので
    // スタックが溢れないようにポップしておく
    if (node->ty < ND_COMP_STMT)
      printf("\tpop rax\n");
  }

  // エピローグ
  // 最後の式の結果が RAX に残っているのでそれが返り値になる
  if (cmdln_flg == true) {
    printf("\tmov rsp, rbp\n");
    printf("\tpop rbp\n");
    printf("\tret\n");
  }

  return EXIT_SUCCESS;
}
