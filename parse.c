#include "9cc.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void program();
Node *assign();
Node *expr();
Node *mul();
Node *term();

void parse() { program(); }

/*
program: assign program'
program': ε | assign program'
*/
void program() {
  int i = 0;
  while (((Token *)(tokens->data[pos]))->ty != TK_EOF) {
    code[i] = assign();
    i++;
  }
  code[i] = NULL;
}

/*
assign: expr assign' ";"
assign': ε | "="  expr assign'
assign':     "==" expr assign'
assign':     "!=" expr assign'
*/
Node *assign() {
  Node *lhs = expr();

  if (((Token *)tokens->data[pos])->ty == '=') {
    pos++;
    return new_node('=', lhs, assign());
  }

  if (((Token *)tokens->data[pos])->ty == TK_EQ) {
    pos++;
    return new_node(ND_EQ, lhs, assign());
  }
  if (((Token *)tokens->data[pos])->ty == TK_NE) {
    pos++;
    return new_node(ND_NE, lhs, assign());
  }

  if (((Token *)tokens->data[pos])->ty == ';') {
    pos++;
    return lhs;
  }

  error("unexpected token: %s\n", ((Token *)tokens->data[pos])->input);
}

/*
expr: mul | mul "+" expr | mul "-" expr
--
expr: mul ( "+" expr | "-" expr | ε )
*/
Node *expr() {
  Node *lhs = mul();

  if (((Token *)tokens->data[pos])->ty == '+') {
    pos++;
    return new_node('+', lhs, expr());
  }

  if (((Token *)tokens->data[pos])->ty == '-') {
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

  if (((Token *)tokens->data[pos])->ty == '*') {
    pos++;
    return new_node('*', lhs, mul());
  }

  if (((Token *)tokens->data[pos])->ty == '/') {
    pos++;
    return new_node('/', lhs, mul());
  }

  return lhs; // epsilon
}

/*
term: NUMBER | IDENT | "(" expr ")"
 */
Node *term() {
  if (((Token *)tokens->data[pos])->ty == TK_NUM)
    return new_node_num(((Token *)tokens->data[pos++])->val);
  if (((Token *)tokens->data[pos])->ty == TK_IDENT) {
    return new_node_id(((Token *)tokens->data[pos++])->input[0]);
  }
  if (((Token *)tokens->data[pos])->ty == '(') {
    pos++;
    Node *node = expr();
    if (((Token *)tokens->data[pos])->ty != ')')
      error("対応する閉じカッコがありません: %s\n",
            ((Token *)tokens->data[pos])->input);
    pos++;
    return node;
  }

  error("unexpected token: %s\n", ((Token *)tokens->data[pos])->input);
}

#ifdef UNIT_TEST
Vector *tokens;
int pos = 0;
Node *code[100];

int main() {
  tokens = new_vector();
  char buf[100];

  strcpy(buf, "a=b=8;");

  tokenize(buf);
  parse();

  printf("%d\n", code[0]->ty);
  printf("%c\n", code[0]->lhs->name);
  printf("%d\n", code[0]->rhs->ty);
  printf("%c\n", code[0]->rhs->lhs->name);
  printf("%d\n", code[0]->rhs->rhs->val);

  return 0;
}
#endif
