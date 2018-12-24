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

void tokenize() {
  Token *token;
  char buf[100];
  int c;

  if(cmdln_flg == true)
    buf_ptr = ARGV[1];
  
  while (true) {
    token = (Token *)malloc(sizeof(Token));
    c = mygetc();
    
    // ignore space
    if (isspace(c)) {
      continue;
    }

    // 演算子, ";"
    switch (c) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '(':
    case ')':
    case ';':
      token->ty = c;
      //      token->input = p;
      vec_push(tokens, token);
      continue;
    }

    // "=", "=="
    if (c == '=') {
      c = mygetc();
      if (c == '=') {
        token->ty = TK_EQ;
	//        token->input = p;
        vec_push(tokens, token);
      } else {
	myungetc(c);	
        token->ty = '=';
	//        token->input = p;
        vec_push(tokens, token);
      }
      continue;
    }

    // "!="
    if (c == '!') {
      c = mygetc();
      if( c == '=') {
	token->ty = TK_NE;
	//token->input = p;
	vec_push(tokens, token);
	continue;
      } else {
	token->ty = c;
	//        token->input = p;
	vec_push(tokens, token);
	myungetc(c);
      }
      continue;
    }

    // NUMBER
    if (isdigit(c)) {
      //      token->input = p;
      token->val = c - '0';
      c = mygetc();
      while( isdigit(c) ) {
	token->val = token->val * 10 + c - '0';
	c = mygetc();
      }
      myungetc(c);
      token->ty = TK_NUM;      
      vec_push(tokens, token);
      continue;
    }

    // 識別子
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
      
      token->name = (char *)malloc(( strlen(buf) + 1) * sizeof(char));
      strcpy(token->name, buf);

      SYM_REC *rec;
      if((rec = lookup(token->name)) == NULL) {
	insert(token->name, TK_IDENT);
	token->ty = TK_IDENT;
	//	token->input = p;
      } else {
	token->ty = rec->token;
      }
      vec_push(tokens, token);

      continue;
      
    }
    if( c == EOF ) {
      token = (Token *)malloc(sizeof(Token));
      token->ty = TK_EOF;
      //  token->input = p;
      vec_push(tokens, token);
      return;
    }
    error("トークナイズできません: (%c)\n", c);
  }
}

int mygetc() {
  if(cmdln_flg == true) 
    return (*buf_ptr == '\0') ? EOF : *buf_ptr++;
  return fgetc(yyin);
}

int myungetc(int c){
  if(cmdln_flg == true) {
    *--buf_ptr = c;
    return *buf_ptr;
  }
  return ungetc(c, yyin);
}
