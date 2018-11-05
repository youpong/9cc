#include <stdlib.h>
#include <stdio.h>
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

Node_Ite *new_ite(Node *start) {
  Node_Ite *ite = malloc(sizeof(Node_Ite));
  ite->cur = start;
  return ite;
}

// returns NULL, if end.
Node *next_node(Node_Ite *ite) {
  if (ite->cur->ty == ND_NUM) {
    return ite->cur;
  }
  // ite->cur->ty != ND_NUM
  return NULL; // TODO
}

#ifdef UNIT_TEST

void test10() {
  Node *node = new_node_num(27);
  Node_Ite *ite = new_ite(node);
  
  Node *i = next_node(ite);
  printf("%d\n", i->val);
}

int main() {
  test10();
  return 0;
}

#endif
