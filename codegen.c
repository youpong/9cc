#include "9cc.h"
#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void gen_lval(Node *node) {
  if (node->ty == ND_IDENT) {
    SYM_REC *rec = lookup(node->name);
    printf("\tmov rax, rbp\n");
    printf("\tsub rax, %d\n", (rec->addr + 1) * 8);
    printf("\tpush rax\n");
    return;
  }
  error("変数ではありません\n");
}

void gen(Node *node) {
  if (node->ty == ND_IF) {
    // ラベルの作成 l
    // cond
    gen(node->cond);
    // スタックトップの値でlへ条件分岐
    printf("\tpop rdi\n");
    printf("\tcmp rdi,0\n");
    printf("\tje L0\n");
    
    // then
    gen(node->then);
    // ラベルの印字
    printf("L0:\n");
    
    return;
  }
  
  if (node->ty == ND_NUM) {
    printf("\tpush %d\n", node->val);
    return;
  }

  if (node->ty == ND_IDENT) {
    gen_lval(node);
    printf("\tpop rax\n");
    printf("\tmov rax, [rax]\n");
    printf("\tpush rax\n");
    return;
  }

  if (node->ty == '=') {
    gen_lval(node->lhs);
    gen(node->rhs);

    printf("\tpop rdi\n");
    printf("\tpop rax\n");
    printf("\tmov [rax], rdi\n");
    printf("\tpush rdi\n");
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("\tpop rdi\n");
  printf("\tpop rax\n");

  switch (node->ty) {
  case '+':
    printf("\tadd rax, rdi\n");
    break;
  case '-':
    printf("\tsub rax, rdi\n");
    break;
  case '*':
    printf("\tmul rdi\n");
    break;
  case '/':
    printf("\tmov rdx, 0\n");
    printf("\tdiv rdi\n");
    break;
  case ND_EQ:
    printf("\tcmp rdi, rax\n");
    printf("\tsete al\n");
    printf("\tmovzb rax, al\n");
    break;
  case ND_NE:
    printf("\tcmp rdi, rax\n");
    printf("\tsetne al\n");
    printf("\tmovzb rax, al\n");
    break;
  default:
    error("unexpected type of node\n");
  }

  printf("\tpush rax\n");
}

#ifdef UNIT_TEST
int pos;
Vector *tokens;
Map *var_tab;
int var_cnt;

void test10() { gen(new_node_num(1047)); }

void test20() { gen(new_node('+', new_node_num(1), new_node_num(2))); }

void test30() { gen(new_node('/', new_node_num(3), new_node_num(4))); }

void test40() {
  Node *node = new_node('+', new_node_num(5), new_node_num(6));
  gen(new_node('-', node, new_node_num(7)));
}

void test41() {
  Node *node = new_node('+', new_node_num(8), new_node_num(9));
  gen(new_node('-', new_node_num(10), node));
}

void test50() { gen(new_node('*', new_node_num(11), new_node_num(12))); }

void test51() { gen(new_node('/', new_node_num(13), new_node_num(14))); }

void test100() {
  Node *me = new_node('+', new_node_num(15), new_node_num(16));
  Node *fater = new_node('-', me, new_node_num(18));
  Node *grand = new_node('*', fater, new_node_num(19));
  gen(grand);
}

int main() {
  tokens = new_vector();
  var_tab = new_map();
  test100();

  return 0;
}
#endif
