#include "9cc.h"
#include "util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void program();
Node *assign();
Node *expr();
Node *mul();
Node *term();
static void match(int);

/*
 *
 */
void parse() { program(); }

Token *lookahead;
/*
program: assign program'
program': ε | assign program'
*/
void program() {
  lookahead = (Token *)tokens->data[0];

  while (lookahead->ty != TK_EOF)
    vec_push(code, assign());
}

/*
assign: expr assign' ";"
assign': ε | "="  expr assign'
assign':     "==" expr assign'
assign':     "!=" expr assign'
*/
Node *assign() {
  Node *lhs = expr();

  if (lookahead->ty == '=') {
    match('=');
    return new_node('=', lhs, assign());
  }

  if (lookahead->ty == TK_EQ) {
    match(TK_EQ);
    return new_node(ND_EQ, lhs, assign());
  }

  if (lookahead->ty == TK_NE) {
    match(TK_NE);
    return new_node(ND_NE, lhs, assign());
  }

  if (lookahead->ty == ';') {
    match(';');
    return lhs;
  }

  error("unexpected token: %s", lookahead->input);
}

/*
 * expr:  expr "+" mul | expr "-" mul | mul
 * --
 * expr:  mul rest2
 * rest2: "+" mul rest2 | "-" mul rest2 | ε
 * --
 * expr:  mul ("+" mul | "-" mul )*
 */
Node *expr() {
  Node *lhs = mul();
  while (true) {
    if (lookahead->ty == '+') {
      match('+');
      lhs = new_node('+', lhs, mul());
    } else if (lookahead->ty == '-') {
      match('-');
      lhs = new_node('-', lhs, mul());
    } else
      break;
  }
  return lhs;
}

/*
 * mul:   mul "*" term | mul "/" term | term
 * --
 * mul:   term rest1
 * rest1: "*" term rest1 | "/" term rest1 | ε
 * --
 * mul: term ("*" term | "/" term)*
 */
Node *mul() {
  Node *lhs = term();
  while (true) {
    if (lookahead->ty == '*') {
      match('*');
      lhs = new_node('*', lhs, term());
    } else if (lookahead->ty == '/') {
      match('/');
      lhs = new_node('/', lhs, term());
    } else
      break;
  }
  return lhs;
}

/*
term: NUMBER | IDENT | "(" expr ")"
 */
Node *term() {
  Node *node;
  if (lookahead->ty == TK_NUM) {
    node = new_node_num(lookahead->val);
    match(TK_NUM);
    return node;
  }
  if (lookahead->ty == TK_IDENT) {
    if (map_get(var_tab, lookahead->name) == NULL) {
      int *num = (int *)malloc(sizeof(int));
      *num = var_cnt++;
      map_put(var_tab, lookahead->name, (void *)num);
    }
    node = new_node_id(lookahead->name);
    match(TK_IDENT);
    return node;
  }
  if (lookahead->ty == '(') {
    match('(');
    node = expr();
    match(')');
    return node;
  }
  error("unexpected token: %s", lookahead->input);
}

static void match(int ty) {
  if (lookahead->ty != ty)
    error("unexpected %c : expected %c", lookahead->ty, ty);

  lookahead = (Token *)tokens->data[++pos];
}

#ifdef UNIT_TEST
Vector *tokens;
int pos = 0;
Vector *code;
Map *var_tab;
int var_cnt;

int main() {
  tokens = new_vector();
  code = new_vector();
  var_tab = new_map();
  char buf[100];

  strcpy(buf, "a=b=8;");

  tokenize(buf);
  parse();

  Node *node = (Node *)code->data[0];
  // TODO: test
  printf("%d\n", node->ty);
  printf("%s\n", node->lhs->name);
  printf("%d\n", node->rhs->ty);
  printf("%s\n", node->rhs->lhs->name);
  printf("%d\n", node->rhs->rhs->val);

  return 0;
}
#endif
