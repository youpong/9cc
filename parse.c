#include <stdlib.h>
#include <stdio.h>
#include "9cc.h"

enum {
  ND_NUM = 256,
};


Node *new_node(int, Node *, Node *);
Node *new_node_num(int);
Node *expr();
Node *mul();
Node *term();
static void error(char *, char *);

Node *new_node(int ty, Node *lhs, Node *rhs) {
  Node *node = malloc(sizeof(Node));
  node->ty = ty;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_NUM;
  node->val = val;
  return node;
}

Node *parse() {
  return expr();
}

Node *expr() {
  Node *lhs = mul();
  char *ops = "+-";
  if (tokens[pos].ty == TK_EOF)
    return lhs;
  for(char *ptr = ops; *ptr != '\0'; ptr++) {
    if (tokens[pos].ty == *ptr) {
      pos++;
      return new_node(*ptr, lhs, expr());
    }
  }
  error("unexpected token: %s", tokens[pos].input);
}
  
Node *mul() {
  char *ops = "*/";
  Node *lhs = term();
  if (tokens[pos].ty == TK_EOF)
    return lhs;
  pos++;
  for (char *ptr = ops; *ptr != '\0'; ptr++) {
    if (tokens[pos].ty == *ptr) {
      pos++;
      return new_node(*ptr, lhs, mul());
    }
  }
  error("unexpected token: %s", tokens[pos].input);
}

Node *term() {
  if (tokens[pos].ty == TK_NUM) 
    return new_node_num(tokens[pos++].val);
  if (tokens[pos].ty == '(') {
    pos++;
    Node *node = expr();
    if(tokens[pos].ty != ')')
      error("対応する閉じカッコがありません: %s", tokens[pos].input);
    pos++;
    return node;
  }
  error("unexpected token: %s", tokens[pos].input);
}

void error(char *fmt, char *input) {
  fprintf(stderr, fmt, input);
  exit(1);
}
