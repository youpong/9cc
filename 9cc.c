#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "9cc.h"

Token tokens[100];
int pos = 0;

void tokenize(char *p) {
  int i = 0;
  while (*p) {
    
    // ignore space
    if (isspace(*p)) {
      p++;
      continue;
    }

    // 演算子, ";"
    switch (*p) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '(':
    case ')':
    case '=':
    case ';':
      tokens[i].ty = *p;
      tokens[i].input = p;
      i++;
      p++;
      continue;
    }

    // NUMBER
    if (isdigit(*p)) {
      tokens[i].ty = TK_NUM;
      tokens[i].input = p;
      tokens[i].val = strtol(p, &p, 10);
      i++;
      continue;
    }

    // 識別子
    if ('a' <= *p && 'z' >= *p) {
      tokens[i].ty = TK_IDENT;
      tokens[i].input = p;
      i++;
      p++;
      continue;
    }
    
    error("トークナイズできません: %s\n", p);
  }
  tokens[i].ty = TK_EOF;
  tokens[i].input = p;
}

Node *code[100];

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "引数の数が正しくありません\n");
    return 1;
  }

  tokenize(argv[1]);
  parse();
  //Node *node = parse();

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
