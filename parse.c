#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "9cc.h"

void program();
Node *assign();
Node *expr();
Node *mul();
Node *term();

void parse() {
  program();
}

/*
program: assign program'
program': ε | assign program'
*/
void program() {
  int i=0;
  while(tokens[pos].ty != TK_EOF) {
    code[i] = assign();
    i++;
  }
  code[i] = NULL;
}

/*
assign: expr assign' ";"
assign': ε | "=" expr assign'
*/
Node *assign() {
  // TODO: test
  Node *lhs = expr();

  if (tokens[pos].ty == '=') {
    pos++;
    return new_node('=', lhs, assign());
  }
  
  if (tokens[pos].ty == ';') {
    pos++;
    return lhs;
  }
  error("unexpected token: %s\n", tokens[pos].input);
}

/*
expr: mul | mul "+" expr | mul "-" expr
--
expr: mul ( "+" expr | "-" expr | ε )
*/
Node *expr() {
  Node *lhs = mul();

  if (tokens[pos].ty == '+') {
    pos++;
    return new_node('+', lhs, expr());
  }
  
  if (tokens[pos].ty == '-') {
    pos++;
    return new_node('-', lhs, expr());
  }

  return lhs; // epsilon
}

/*
mul: term | term "*" mul | term "/" mul
--
mul: term ( "*" mul | "/" mul | ε )
 */
Node *mul() {
  Node *lhs = term();

  if (tokens[pos].ty == '*') {
    pos++;
      return new_node('*', lhs, mul());
  }
  
  if (tokens[pos].ty == '/') {
    pos++;
    return new_node('/', lhs, mul());
  }

  return lhs; // epsilon
}

/*
term: NUMBER | IDENT | "(" expr ")"
 */
Node *term() {
  if (tokens[pos].ty == TK_NUM) 
    return new_node_num(tokens[pos++].val);
  if (tokens[pos].ty == TK_IDENT) {
    return new_node_id(tokens[pos++].input[0]);
  }
  if (tokens[pos].ty == '(') {
    pos++;
    Node *node = expr();
    if(tokens[pos].ty != ')')
      error("対応する閉じカッコがありません: %s\n", tokens[pos].input);
    pos++;
    return node;
  }
  
  error("unexpected token: %s\n", tokens[pos].input);
}

#ifdef UNIT_TEST
Token tokens[100];
int pos = 0;
Node *code[100];

int main() {
  char buf[100];
  
  strcpy(buf, "a=b=8;");
  
  tokenize(buf);
  parse();
  
  printf("%d\n", code[0]->ty);
  printf("%c\n", code[0]->lhs->name);  
  printf("%d\n", code[0]->rhs->ty);
  printf("%c\n", code[0]->rhs->lhs->name);
  printf("%d\n", code[0]->rhs->rhs->val);
  /*  
  printf("%d\n", code[0]->lhs->lhs->ty);
  printf("%c\n", code[0]->lhs->lhs->name);
  printf("%d\n", code[0]->lhs->rhs->ty);
  printf("%c\n", code[0]->lhs->rhs->name);

  printf("%d\n", code[0]->rhs->ty);
  printf("%d\n", code[0]->rhs->val);
  */
  return 0;
}
#endif
