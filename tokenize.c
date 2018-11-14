#include "9cc.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

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
    case ';':
      tokens[i].ty = *p;
      tokens[i].input = p;
      i++;
      p++;
      continue;
    }

    if (*p == '=') {
      if (*(p + 1) == '=') {
        tokens[i].ty = TK_EQ;
        tokens[i].input = p;
        i++;
        p += 2;
      } else {
        tokens[i].ty = *p;
        tokens[i].input = p;
        i++;
        p++;
      }
      continue;
    }

    if (*p == '!' && *(p + 1) == '=') {
      tokens[i].ty = TK_NE;
      tokens[i].input = p;
      i++;
      p += 2;
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
