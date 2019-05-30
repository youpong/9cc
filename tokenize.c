#include "9cc.h"
#include "util.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int mygetc();
static int myungetc(int);

enum {
  IN_COMMENT = 1,
  NOT_IN_COMMENT = 0,
};

void tokenize() {
  Token *token;
  char buf[100];
  int status = NOT_IN_COMMENT;
  int c;

  yyin = fopen(ARGV[1], "r");
  if (yyin == NULL)
    error("file cannot open");

  while (true) {
    token = malloc(sizeof(Token));
    c = mygetc();

    // comment 終端処理
    if (status == IN_COMMENT) {
      if (c == '*') {
        c = mygetc();
        if (c == '/') {
          status = NOT_IN_COMMENT;
          continue;
        }
        myungetc(c);
      }
      continue;
    }

    // commnent 開始処理
    if (c == '/') {
      c = mygetc();
      if (c == '*') {
        status = IN_COMMENT;
        continue;
      }
      // comment の開始記号ではない単なる '/'
      myungetc(c);
      token->ty = '/';
      vec_push(tokens, token);
      continue;
    }

    // ignore space
    if (isspace(c)) {
      continue;
    }

    // 演算子, ";"
    switch (c) {
    case '+':
    case '-':
    case '*':
    case '(':
    case ')':
    case '{':
    case '}':
    case ',':
    case ';':
      token->ty = c;
      vec_push(tokens, token);
      continue;
    }

    // "=", "=="
    if (c == '=') {
      c = mygetc();
      if (c == '=') {
        token->ty = TK_EQ;
        vec_push(tokens, token);
      } else {
        myungetc(c);
        token->ty = '=';
        vec_push(tokens, token);
      }
      continue;
    }

    // "!="
    if (c == '!') {
      c = mygetc();
      if (c == '=') {
        token->ty = TK_NE;
        vec_push(tokens, token);
      } else {
        token->ty = c;
        vec_push(tokens, token);
        myungetc(c);
      }
      continue;
    }

    // NUMBER
    if (isdigit(c)) {
      token->val = c - '0';
      c = mygetc();
      while (isdigit(c)) {
        token->val = token->val * 10 + c - '0';
        c = mygetc();
      }
      myungetc(c);
      token->ty = TK_NUM;
      vec_push(tokens, token);
      continue;
    }

    // identifier or reserved word
    if (isalpha(c)) {
      char *p = buf;
      *p++ = c;
      c = mygetc();
      while (isalnum(c)) {
        *p++ = c;
        c = mygetc();
      }
      myungetc(c);
      *p = '\0';

      token->name = malloc((strlen(buf) + 1) * sizeof(char));
      strcpy(token->name, buf);

      SYM_REC *rec;
      token->ty = TK_IDENT;
      if ((rec = query_reserved(token->name)) != NULL) {
        token->ty = rec->token;
      }

      vec_push(tokens, token);
      continue;
    }

    if (c == EOF) {
      token->ty = TK_EOF;
      vec_push(tokens, token);
      return;
    }

    error("トークナイズできません: (%c)\n", c);
  }
}

int mygetc() {
  return fgetc(yyin);
}

int myungetc(int c) {
  return ungetc(c, yyin);
}
