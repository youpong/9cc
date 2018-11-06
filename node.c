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
  ite->top = 0;
  ite->cur = start;
  return ite;
}

// returns NULL, if end.
Node *next_node(Node_Ite *ite) {
  Node *ret;

  if (ite->cur == NULL ) {
    return NULL;
  }

  // TODO
  // case return LHS
  // case return RHS
  // case return ty
  if (ite->cur->ty == ND_NUM) {
    ret = ite->cur;
    ite->cur = NULL;
    return ret;
  }
  
  // ite->cur->ty != ND_NUM
  //while( ite->cur->ty != ND_NUM ) {
  //  push( cur, ite->stack )
  ite->stack[ite->top] = ite->cur;
  ite->top++;
  ite->cur = ite->cur->lhs;
  return ite->cur;
  //}
}

#ifdef UNIT_TEST

void print_node(Node *node) {
  Node *i;
  Node_Ite *ite = new_ite(node);
  while	((i = next_node(ite)) != NULL) {
    if(i->ty == ND_NUM) {
      printf("%d\n", i->val);
      continue;
    }
    printf("%c\n", i->ty);
  }
}

void test10() {
  Node *node = new_node_num(27);

  print_node(node);
}

void test20() {
  Node *node = new_node('+', new_node_num(30), new_node_num(31));

  print_node(node); // 30 31 + 
}  

int main() {
  test10();
  test20();
  
  return 0;
}
#endif
