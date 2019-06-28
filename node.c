#include "9cc.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  node->c_ty = malloc(sizeof(Type));
  node->c_ty->ty = INT;

  node->val = val;
  return node;
}

Node *new_node_id(char *name) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_IDENT;
  node->name = malloc(sizeof(char) * strlen(name));
  strcpy(node->name, name);

  // TODO: test here
  SYM_REC *rec = query_var(name);
  if (rec == NULL)
    error("error: use of undeclared identifier %s", name);

  node->c_ty = rec->ty;

  return node;
}
