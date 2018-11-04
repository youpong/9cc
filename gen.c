#include <stdio.h>
#include "9cc.h"

void gen(Node *node) {
  if (node->ty == ND_NUM) {
    printf("\tmov rax, %d\n", node->val);
  }
  /*  
  if (node->ty == '+') {
    printf("\tmov %d\n", node->lhs->val);
    printf("\tmov %d\n", node->rhs->val);
  }
  */
  return;
}
