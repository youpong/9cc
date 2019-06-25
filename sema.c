#include "9cc.h"
#include <stdlib.h>

static void walk(Node *node) {
  //  printf("###b(%d)\n", node->ty);

  switch (node->ty) {
  case ND_FUNC_DEF:
    walk(node->body);
    return;
  case ND_COMP_STMT:
    // printf("###e\n");
    for (int i = 0; i < node->stmts->len; i++)
      walk(vec_at(node->stmts, i));
    return;
  case '+':
    //    printf("###c\n");
    walk(node->lhs);
    walk(node->rhs);
    //    printf("###%d\n", node->lhs->c_ty->ty);
    if (node->lhs->c_ty->ty == PTR) {
      Node *n = malloc(sizeof(Node));
      n->ty = '*';
      n->lhs = node->rhs;
      n->rhs = new_node_num(8); // sizeof(void *)
      node->rhs = n;
      //      printf("###\n");
    }
    return;
  case '-':
    // TODO
    return;
  case '=':
    //    printf("###d\n");
    walk(node->lhs);
    walk(node->rhs);
    return;
  case ND_NUM:
    // parse で決定するべき?
    node->c_ty = malloc(sizeof(Type));
    node->c_ty->ty = INT;
    return;
  case ND_IDENT:
    // TODO:
    return;
  }
}

void sema(Vector *code) {

  for (int i = 0; i < code->len; i++) {
    Node *node = vec_at(code, i);
    walk(node);
    /*
    if (node->ty == '+') {
      //
    } else {
      // no-op
    }
    */
  }
}
