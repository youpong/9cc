#include "9cc.h"
#include "util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void program();
static Node *expr();
static Node *assign();
static Node *logical();
static Node *add();
static Node *mul();
static Node *term();
static void match(int);

Token *lookahead;

/*
 *
 */
void parse() { program(); }

/*
program: program expr | ε
*/
static void program() {
  lookahead = (Token *)tokens->data[0];

  while (lookahead->ty != TK_EOF) {
    vec_push(code, expr());
    match(';');
  }
}

/*
 */
static Node *expr() {
  return assign();
}  

/*
production rules

(1) original
assign: logical | logical "=" assign 
*/
static Node *assign() {
  Node *lhs = logical();
  
  if (lookahead->ty == '=') {
    match('=');
    return new_node('=', lhs, assign());
  }

  return lhs;
}

/*
(1) original
logical: logical "==" add | logical "!=" add
(3) 
logical: add ( "==" add | "!=" add)*
*/
static Node *logical() {
  Node *lhs = add();
  while(true)
    if(lookahead->ty == TK_EQ) {
      match(TK_EQ);
      lhs = new_node(TK_EQ, lhs, add());
    } else if(lookahead->ty == TK_NE) {
      match(TK_NE);
      lhs = new_node(TK_NE, lhs, add());
    } else
      break;
  
  return lhs;
}

/*
 * Production Rules
 *
 * (1) original
 * add:  add "+" mul | add "-" mul | mul 
 *
 * (2) elimination of left recursion
 * add:  mul rest2
 * rest2: "+" mul rest2 | "-" mul rest2 | ε
 *
 * (3) elimination of non-terminal rest2
 * add:  mul ("+" mul | "-" mul )*
 * "*" はカッコで囲まれた生成規則を0回以上繰り返す
 *
 * 左結合を実現するため、
 * トークンを読むにつれノードを左に伸ばす
 */
static Node *add() {
  Node *lhs = mul();
  while (true) 
    if (lookahead->ty == '+') {
      match('+');
      lhs = new_node('+', lhs, mul());
    } else if (lookahead->ty == '-') {
      match('-');
      lhs = new_node('-', lhs, mul());
    } else
      break;

  return lhs;
}

/*
 * Production Rule
 *
 * (1) original
 * mul:   mul "*" term | mul "/" term | term
 *
 * (2) elemination of left recursion
 * mul:   term rest1
 * rest1: "*" term rest1 | "/" term rest1 | ε
 *
 * (3) elimination of non-terminal rest1
 * mul: term ("*" term | "/" term)*
 * "*" はカッコで囲まれた生成規則を0回以上繰り返す
 *
 * 左結合を実現するため、
 * トークンを読むにつれノードを左に伸ばす
 */
static Node *mul() {
  Node *lhs = term();
  while (true) 
    if (lookahead->ty == '*') {
      match('*');
      lhs = new_node('*', lhs, term());
    } else if (lookahead->ty == '/') {
      match('/');
      lhs = new_node('/', lhs, term());
    } else
      break;

  return lhs;
}

/*
 * production rule
 * term: NUMBER | IDENT | "(" expr ")"
 */
static Node *term() {
  Node *node;
  switch (lookahead->ty) {
  case TK_NUM:
    node = new_node_num(lookahead->val);
    match(TK_NUM);
    break;
  case TK_IDENT:
    node = new_node_id(lookahead->name);
    match(TK_IDENT);
    break;
  case '(':
    match('(');
    node = expr();
    match(')');
    break;
  default:
    error("unexpected token: %s", lookahead->input);
  }
  return node;
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
