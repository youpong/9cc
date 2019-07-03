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
  case ND_ADDRESS_OF:
    // TODO: test
    walk(node->lhs);
    // node->lhs->c_ty の ptr となる。
    node->c_ty = malloc(sizeof(Type));
    node->c_ty->ty = PTR;
    node->c_ty->ptr_to = node->lhs->c_ty;
    return;
  case ND_DEREFERENCE:
    // TODO: test
    walk(node->lhs);
    node->c_ty = node->lhs->c_ty->ptr_to;
    return;
  case '+':
  case '-':
    walk(node->lhs);
    walk(node->rhs);
    // PTR とあれば ptr to int と仮定している。
    if (node->rhs->c_ty->ty == PTR) {
      Node *n = node->lhs;
      node->lhs = node->rhs;
      node->rhs = n;
    }
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
  case '*':
  case '/':
  case ND_EQ:
  case ND_NE:
    walk(node->lhs);
    walk(node->rhs);
    node->c_ty = malloc(sizeof(Type));
    node->c_ty->ty = INT;
    return;
  case '=':
    walk(node->lhs);
    walk(node->rhs);
    node->c_ty = malloc(sizeof(Type));
    node->c_ty->ty = node->rhs->c_ty->ty;
    return;
  case ND_NUM:
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
