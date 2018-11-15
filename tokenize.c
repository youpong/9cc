#include "9cc.h"
#include "util.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void tokenize(char *p) {
  Token *token;

  while (*p) {

    // ignore space
    if (isspace(*p)) {
      p++;
      continue;
    }

    token = malloc(sizeof(Token));

    // 演算子, ";"
    switch (*p) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '(':
    case ')':
    case ';':
      token->ty = *p;
      token->input = p;
      vec_push(tokens, token);
      p++;
      continue;
    }

    if (*p == '=') {
      if (*(p + 1) == '=') {
        token->ty = TK_EQ;
        token->input = p;
        vec_push(tokens, token);
        p += 2;
      } else {
        token->ty = *p;
        token->input = p;
        vec_push(tokens, token);
        p++;
      }
      continue;
    }

    if (*p == '!' && *(p + 1) == '=') {
      token->ty = TK_NE;
      token->input = p;
      vec_push(tokens, token);
      p += 2;
      continue;
    }

    // NUMBER
    if (isdigit(*p)) {
      token->ty = TK_NUM;
      token->input = p;
      token->val = strtol(p, &p, 10);
      vec_push(tokens, token);
      continue;
    }

    // 識別子
    if ('a' <= *p && 'z' >= *p) {
      token->ty = TK_IDENT;
      token->input = p;
      vec_push(tokens, token);
      p++;
      continue;
    }

    error("トークナイズできません: %s\n", p);
  }
  token = malloc(sizeof(Token));
  token->ty = TK_EOF;
  token->input = p;
  vec_push(tokens, token);
}
