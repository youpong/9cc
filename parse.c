#include "9cc.h"
#include "util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Node *func_def();
static Node *compound_stmt();
static Node *stmt();
static Node *var_def();
static Node *if_stmt();
static Node *while_stmt();
static Node *break_stmt();
static Node *continue_stmt();
static Node *return_stmt();
static Node *expr();
static Node *assign();
static Node *logical();
static Node *add();
static Node *mul();
static Node *term();
static void match(int);

// extern
Token *lookahead;

// static
static Vector *breaks;
static Vector *continues;

/*
 * parse: func_def*
 */
void parse() {
  breaks = new_vector();
  continues = new_vector();

  lookahead = (Token *)tokens->data[0];
  while (lookahead->ty != TK_EOF) 
    vec_push(code, func_def());
}

/*
 * func_def: INT IDENT '(' params_opt ')' '{' stmt_list '}'
 * params_opt: ε | INT IDENT (',' INT IDENT)*
 */
static Node *func_def() {
  Node *node = malloc(sizeof(Node));
  node->params = new_vector();

  node->ty = ND_FUNC_DEF;

  match(TK_INT);
  node->name = strdup(lookahead->name);
  match(TK_IDENT);
  match('(');

  if (lookahead->ty != ')') {
    while (true) {
      match(TK_INT);
      vec_push(node->params, new_node_id(lookahead->name));
      match(TK_IDENT);
      if (lookahead->ty == ')')
        break;
      match(',');
    }
  }
  match(')');
  node->body = compound_stmt();

  return node;
}

/*
 * compound_stmt: '{' stmt* '}'
 */
static Node *compound_stmt() {
  Node *node = new_node(ND_COMP_STMT, NULL, NULL);
  node->stmts = new_vector();

  match('{');
  while (lookahead->ty != '}') {
    vec_push(node->stmts, stmt());
  }
  match('}');

  return node;
}

/*
 * stmt: var_def
 *     | if_stmt
 *     | while_stmt
 *     | break_stmt
 *     | continue_stmt
 *     | return_stmt
 *     | compound_stmt
 *     | expr ';'
 *     ;
 */
static Node *stmt() {
  Node *node;

  if (lookahead->ty == TK_INT)
    return var_def();
  if (lookahead->ty == TK_IF)
    return if_stmt();
  if (lookahead->ty == TK_WHILE)
    return while_stmt();
  if (lookahead->ty == TK_BREAK)
    return break_stmt();
  if (lookahead->ty == TK_CONTINUE)
    return continue_stmt();
  if (lookahead->ty == TK_RETURN)
    return return_stmt();
  if (lookahead->ty == '{')
    return compound_stmt();

  node = expr();
  match(';');
  return node;
}

/*
 * var_def: INT IDENT ';'
 */
static Node *var_def() {
  Node *node = malloc(sizeof(Node));

  node->ty = ND_VAR_DEF;
  match(TK_INT);
  match(TK_IDENT);
  match(';');

  return node;
}

/*
 * if_stmt: IF '(' expr ')' stmt
 *        | IF '(' expr ')' stmt ELSE stmt
 *        ;
 */
static Node *if_stmt() {
  Node *node = malloc(sizeof(Node));

  node->ty = ND_IF;
  match(TK_IF);
  match('(');
  node->cond = expr();
  match(')');
  node->then = stmt();

  if (lookahead->ty == TK_ELSE) {
    match(TK_ELSE);
    node->els = stmt();
  }

  return node;
}

/*
 * while_stmt: WHILE '(' expr ')' stmt
 *           ;
 */
static Node *while_stmt() {
  Node *node = malloc(sizeof(Node));

  vec_push(breaks, node);
  vec_push(continues, node);

  node->ty = ND_WHILE;
  match(TK_WHILE);
  match('(');
  node->cond = expr();
  match(')');
  node->body = stmt();

  vec_pop(breaks);
  vec_pop(continues);

  return node;
}

/*
 * break_stmt: BREAK ';'
 *           ;
 */
static Node *break_stmt() {
  Node *node = malloc(sizeof(Node));

  node->ty = ND_BREAK;
  node->target = vec_last(breaks);
  match(TK_BREAK);
  match(';');

  return node;
}

/*
 * continue_stmt: CONTINUE ';'
 *              ;
 */
static Node *continue_stmt() {
  Node *node = malloc(sizeof(Node));

  node->ty = ND_CONTINUE;
  node->target = vec_last(continues);
  match(TK_CONTINUE);
  match(';');

  return node;
}

/*
 * TOBE
 * return_stmt: RETURN expr? ';'
 * --
 * ASIS
 * return_stmt: RETURN expr ';'
 */
static Node *return_stmt() {
  Node *node = malloc(sizeof(Node));

  node->ty = ND_RETURN;
  match(TK_RETURN);
  node->lhs = expr();
  match(';');

  return node;
}

/*
 * expr: assign
 */
static Node *expr() {
  return assign();
}

/*
production rules

(1) original
assign: logical | logical '=' assign
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
 TK_EQ ==
 TK_NE !=

(1) original
logical: logical (TK_EQ|TK_NE) add
(3)
logical: add (TK_EQ add|TK_NE add)*
*/
static Node *logical() {
  Node *lhs = add();
  while (true)
    if (lookahead->ty == TK_EQ) {
      match(TK_EQ);
      lhs = new_node(ND_EQ, lhs, add());
    } else if (lookahead->ty == TK_NE) {
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
 * add:  add '+' mul | add '-' mul | mul
 *
 * (2) elimination of left recursion
 * add:  mul rest2
 * rest2: '+' mul rest2 | '-' mul rest2 | ε
 *
 * (3) elimination of non-terminal rest2
 * add:  mul ('+' mul |'-' mul)*
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
 * mul:   mul '*' term | mul '/' term | term
 *
 * (2) elemination of left recursion
 * mul:   term rest1
 * rest1: '*' term rest1 | '/' term rest1 | ε
 *
 * (3) elimination of non-terminal rest1
 * mul: term ('*' term | '/' term)*
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
 * term: NUMBER
 *     | IDENT | IDENT '(' args_opt ')'
 *     | '(' expr ')'
 * args_opt: ε | expr ( ',' expr )*
 */
static Node *term() {
  Node *node;
  char *name;
  switch (lookahead->ty) {
  case TK_NUM:
    node = new_node_num(lookahead->val);
    match(TK_NUM);
    break;
  case TK_IDENT:
    name = lookahead->name;
    match(TK_IDENT);
    if (lookahead->ty == '(') {
      node = malloc(sizeof(Node));
      node->args = new_vector();

      node->ty = ND_FUNC_CALL;
      node->name = name;

      match('(');
      if (lookahead->ty != ')') {
        while (true) {
          vec_push(node->args, expr());
          if (lookahead->ty == ')')
            break;
          match(',');
        }
      }
      match(')');
      //      printf("len(%d)\n", node->args->len);
    } else
      node = new_node_id(name);
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
