#include "9cc.h"
#include <stdio.h>

static void p_node(Node *);

void p_tree(Node *node) {
  if (node->lhs != NULL) {
    p_node(node);
    printf(" -- ");
    p_node(node->lhs);
    printf(" [label = \"lhs\"];\n");
  }
  if (node->rhs != NULL) {
    p_node(node);
    printf(" -- ");
    p_node(node->rhs);
    printf(" [label = \"rhs\"];\n");
  }

  if (node->lhs != NULL)
    p_tree(node->lhs);
  if (node->rhs != NULL)
    p_tree(node->rhs);

  p_node(node);
  printf("\n");
}

static void p_node(Node *node) {
  switch (node->ty) {
  case ND_NUM:
    printf("%d", node->val);
    break;
  case ND_IDENT:
    printf("%s", node->name);
    break;
  default:
    printf("\"%c\"", node->ty);
  }
}
