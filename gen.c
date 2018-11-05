#include <stdio.h>
#include "9cc.h"

void gen(Node *node) {
  if (node->ty == ND_NUM) {
    printf("\tmov rax, %d\n", node->val);
  }

  if (node->ty == '+') {
    printf("\tmov rax, %d\n", node->lhs->val);
    printf("\tadd rax, %d\n", node->rhs->val);
  }

  return;
}

#ifdef UNIT_TEST
int pos;
Token tokens[100];
	     
int main() {
  Node *node;
  node = new_node_num(1047);
  gen(node);

  node = new_node('+', new_node_num(1), new_node_num(2));
  gen(node);
  return 0;
}
#endif
