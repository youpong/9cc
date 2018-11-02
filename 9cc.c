#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
  TK_NUM = 256,
  TK_EOF,
};

typedef struct {
  int ty; // type of token
  int val; // ty が TK_NUM の場合のその値
  char *input; // token string(for error message)
} Token;

Token tokens[100];

void tokenize(char *p) {
  int i = 0;
  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }
    if (*p == '+' || *p == '-') {
      tokens[i].ty = *p;
      tokens[i].input = p;
      i++;
      p++;
      continue;
    }
    if (isdigit(*p)) {
      tokens[i].ty = TK_NUM;
      tokens[i].input = p;
      tokens[i].val = strtol(p, &p, 10);
      i++;
      continue;
    }
    fprintf(stderr, "トークナイズできません: %s\n", p);
    exit(1);
  }
  tokens[i].ty = TK_EOF;
  tokens[i].input = p;
}

void error(int i) {
  fprintf(stderr, "unexpected token: %s\n", tokens[i].input);
  exit(1);
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "引数の数が正しくありません\n");
    return 1;
  }

  tokenize(argv[1]);
  
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  if (tokens[0].ty != TK_NUM)
    error(0);
  
  printf("\tmov rax, %d\n", tokens[0].val);

  int i = 1;
  while (tokens[i].ty != TK_EOF) {
    if (tokens[i].ty == '+') {
      i++;
      if (tokens[i].ty != TK_NUM)
	error(i);
      printf("\tadd rax, %d\n", tokens[i].val);
      i++;
      continue;
    }
    
    if (tokens[i].ty == '-') {
      i++;
      if (tokens[i].ty != TK_NUM)
	error(i);
      printf("\tsub rax, %d\n", tokens[i].val);
      i++;
      continue;
    }

    error(i);
    return 1;
  }
  
  printf("\tret\n");
  return 0;
}
