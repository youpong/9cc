#include "9cc.h"
#include "util.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Token tokens[100];
int pos = 0;
Node *code[100];

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "引数の数が正しくありません\n");
    return 1;
  }

  if (strcmp(argv[1], "-test") == 0) {
    run_utiltest();
    return 0;
  }

  tokenize(argv[1]);
  parse();

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // プロローグ
  // 変数26個分の領域を確保する
  printf("\tpush rbp\n");
  printf("\tmov rbp, rsp\n");
  printf("\tsub rsp, 208\n");

  for (int i = 0; code[i]; i++) {
    gen(code[i]);
    // 式の評価結果としてスタックに一つの値が残っているはずなので
    // スタックが溢れないようにポップしておく
    printf("\tpop rax\n");
  }

  // エピローグ
  // 最後の式の結果が RAX に残っているのでそれが返り値になる
  printf("\tmov rsp, rbp\n");
  printf("\tpop rbp\n");
  printf("\tret\n");

  return 0;
}
