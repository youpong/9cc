#include "9cc.h"
#include "util.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static char *buf_ptr;
static int mygetc();
static int myungetc(int);

void tokenize(char *p) {
  Token *token;

  if(cmdln_flg == true)
    buf_ptr = p;
  
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

    // "=", "=="
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

    // "!="
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
    if (isalpha(*p)) {

      int len = 1;
      for (char *q = p + 1; isalnum(*q); q++)
        len++;
      token->name = malloc(sizeof(char) * (len + 1));
      strncpy(token->name, p, len + 1);
      token->name[len] = '\0';

      SYM_REC *rec;
      if((rec = lookup(token->name)) == NULL) {
	insert(token->name, TK_IDENT);
	token->ty = TK_IDENT;
	token->input = p;
      } else {
	token->ty = rec->token;
      }
      vec_push(tokens, token);
      
      p += len;
      continue;
    }

    error("トークナイズできません: %s\n", p);
  }
  token = malloc(sizeof(Token));
  token->ty = TK_EOF;
  token->input = p;
  vec_push(tokens, token);
}

int mygetc() {
  if(cmdln_flg == true) 
    return *buf_ptr++;
  return fgetc(yyin);
}

int myungetc(int c){
  if(cmdln_flg == true) {
    *buf_ptr-- = c;
    return *buf_ptr;
  }
  return ungetc(c, yyin);
}
