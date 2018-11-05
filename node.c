#include <stdlib.h>
#include "9cc.h"

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

Node_Ite *setup_ite(Node *start) {
  Node_Ite *ite = malloc(sizeof(Node_Ite));
  ite->cur = start;
  return ite;
}

Node *next_node(Node_Ite *ite) {
  if (ite->cur->ty == ND_NUM) {
    return ite->cur;
  }
  // ite->cur->ty != ND_NUM
  return NULL; // TODO
}

#ifdef UNIT_TEST
int main() {
  return 0;
}
#endif
