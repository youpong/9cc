#include "9cc.h"
#include <stdlib.h>

static void walk(Node *);

// TODO: scale node according to node->c_ty
static Node *scale(Node *node) {
  Node *n = malloc(sizeof(Node));
  n->ty = '*';
  n->lhs = node;
  n->rhs = new_node_num(4); // TODO: sizeof(int)
  walk(n);
  return n;
}

static void walk(Node *node) {
  switch (node->ty) {
  case ND_FUNC_CALL:
    for (int i = 0; i < node->args->len; i++)
      walk(vec_at(node->args, i));
    node->c_ty = malloc(sizeof(Type));
    node->c_ty->ty = INT;
    return;
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
  case ND_UNARY_MINUS:
    walk(node->lhs);
    node->c_ty = node->lhs->c_ty;
    return;
  case '+':
  case '-':
    walk(node->lhs);
    walk(node->rhs);

    if (node->rhs->c_ty->ty == PTR && node->lhs->c_ty->ty == PTR) {
      // TODO: ERR: both of PTR operand is invalid
      exit(1);
    }
    if (node->rhs->c_ty->ty != PTR && node->lhs->c_ty->ty != PTR) {
      // no action
      node->c_ty = node->lhs->c_ty;
      return;
    }
    if (node->lhs->c_ty->ty != PTR) {
      node->lhs = scale(node->lhs);
      node->c_ty = node->rhs->c_ty;
      return;
    }
    if (node->rhs->c_ty->ty != PTR) {
      node->rhs = scale(node->rhs);
      node->c_ty = node->lhs->c_ty;
      return;
    }
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
