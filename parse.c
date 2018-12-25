#include "9cc.h"
#include "util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Node *stmt();
static Node *compound_stmt();
static Node *ifthen();
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
void parse() {
  lookahead = (Token *)tokens->data[0];
  while(lookahead->ty != TK_EOF)
    vec_push(code,  stmt());
}

/*
Production rule
(1) original
program: program expr ";" | ε

(2)
program: ( expr ";" )*
*/
static Node *stmt() {
  Node *node;

  if(lookahead->ty == TK_IF)
    return ifthen();
  else if (lookahead->ty == '{') 
    return compound_stmt();
  else {
    node = expr();
    match(';');
    return node;
  }
}

/*
 * "if" ( cond ) then "else" els
 */
static Node *ifthen() {
  Node *node = (Node *)malloc(sizeof(Node));

  node->ty = ND_IF;
  match(TK_IF);
  match('(');
  node->cond = expr();
  match(')');
  node->then = stmt();

  return node;
}

static Node *compound_stmt() {
  Node *node = new_node(ND_COMP_STMT, NULL, NULL);
  node->stmts = new_vector();
  
  match('{');
  while(lookahead->ty != '}') {
    vec_push(node->stmts, stmt());
  }
  match('}');
  
  return node;
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
      lhs = new_node(ND_EQ, lhs, add());
    } else if(lookahead->ty == TK_NE) {
      match(TK_NE);
      lhs = new_node(ND_NE, lhs, add());
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
    error("unexpected token: %d", lookahead->ty);
  }
  return node;
}

static void match(int ty) {
  if (lookahead->ty != ty)
    error("unexpected %c : expected %c", lookahead->ty, ty);

  lookahead = (Token *)tokens->data[++pos];
}
