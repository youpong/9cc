#include "9cc.h"
#include <stdlib.h>

static void walk(Node *node) {
  switch (node->ty) {
  case ND_FUNC_DEF:
    walk(node->body);
    return;
  case ND_COMP_STMT:
    for (int i = 0; i < node->stmts->len; i++)
      walk(vec_at(node->stmts, i));
    return;
  case '+':
    walk(node->lhs);
    walk(node->rhs);
    // PTR とあれば ptr to int と仮定している。
    if (node->lhs->c_ty->ty == PTR) {
      Node *n = malloc(sizeof(Node));
      n->ty = '*';
      n->lhs = node->rhs;
      n->rhs = new_node_num(4); // sizeof(int)
      node->rhs = n;
    }
    node->c_ty = malloc(sizeof(Type));
    node->c_ty->ty = PTR;
    return;
  case '-':
    // TODO
    return;
  case '=':
    walk(node->lhs);
    walk(node->rhs);
    return;
  case ND_NUM:
    node->c_ty = malloc(sizeof(Type));
    node->c_ty->ty = INT;
    return;
  case ND_IDENT:
    return;
  }
}

void sema(Vector *code) {
  for (int i = 0; i < code->len; i++) {
    Node *node = vec_at(code, i);
    walk(node);
  }
}
