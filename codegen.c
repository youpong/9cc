#include "9cc.h"
#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int label = 0;
// static int rsp_start = 0;
static int rsp_cur = 0;

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
  char *arg_rg[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9", NULL};

  if (node->ty == ND_FUNC_CALL) {

    int len = node->args->len;
    for (int i = 0; i < len; i++) {
      if (arg_rg[i] == NULL)
        error("too match args");
      Node *arg = (Node *)node->args->data[i];
      gen(arg);
      printf("\tpop %s\n", arg_rg[i]);
    }
    int offset = 16 - abs(rsp_cur % 16);
    if (offset % 16 != 0)
      printf("\tsub rsp, %d\n", offset);

    printf("\tcall %s\n", node->name);
    if (offset % 16 != 0)
      printf("\tadd rsp, %d\n", offset);

    printf("\tpush rax\n");
    return;
  }
  if (node->ty == ND_VAR_DEF) {
    printf("\tpush rax\n");
    return;
  }
  if (node->ty == ND_FUNC_DEF) {
    // label
    printf("%s:\n", node->name);

    // pro-logue
    printf("\tpush rbp\n");
    printf("\tmov rbp, rsp\n");
    printf("\tsub rsp, %d\n", var_cnt * 8);
    rsp_cur -= var_cnt * 8;

    // params
    int len = node->params->len;
    for (int i = 0; i < len; i++) {
      Node *param = (Node *)node->params->data[i];
      gen_lval(param);
      printf("\tpush %s\n", arg_rg[i]);

      printf("\tpop rdi\n");
      printf("\tpop rax\n");
      printf("\tmov [rax], rdi\n");
    }

    // body
    gen(node->body);

    // epi-logue
    printf("\tmov rsp, rbp\n");
    printf("\tpop rbp\n");
    printf("\tret\n");
    printf("\tpush rax\n"); 
    return;
  }
  if (node->ty == ND_WHILE) {
    // ラベルの作成 l
    node->label_head = (char *)malloc((3 + 1) * sizeof(char));
    node->label_tail = (char *)malloc((3 + 1) * sizeof(char));
    sprintf(node->label_head, "L%02d", label++); // L0
    sprintf(node->label_tail, "L%02d", label++); // L1

    printf("%s:\n", node->label_head); // L0

    // cond
    gen(node->cond);

    // スタックトップの値でlへ条件分岐
    printf("\tpop rdi\n");
    printf("\tcmp rdi,0\n");
    printf("\tje %s\n", node->label_tail); // L1

    // body
    gen(node->body);
    printf("\tpop rax\n");

    printf("\tjmp %s\n", node->label_head); // L0

    printf("%s:\n", node->label_tail); // L1
    printf("\tpush rax\n");
    return;
  }
  if (node->ty == ND_CONTINUE) {
    printf("\tjmp %s\n", node->target->label_head);
    printf("\tpush rax\n");
    return;
  }
  if (node->ty == ND_BREAK) {
    printf("\tjmp %s\n", node->target->label_tail);
    printf("\tpush rax\n");
    return;
  }
  if (node->ty == ND_IF) {
    // ラベルの作成 l
    int l0 = label++;
    int l1 = label++;

    // cond
    gen(node->cond);
    // スタックトップの値でlへ条件分岐
    printf("\tpop rdi\n");
    printf("\tcmp rdi,0\n");
    printf("\tje L%d\n", l0); // L0 - lelse

    // then
    gen(node->then);
    printf("\tpop rax\n");
    printf("\tjmp L%d\n", l1); // L1 l lthen

    // ラベルの印字
    printf("L%d:\n", l0); // L0
    if (node->els != NULL) {
      gen(node->els);
      printf("\tpop rax\n");
    }

    printf("L%d:\n", l1); // L1
    printf("\tpush rax\n");
    return;
  }
  if (node->ty == ND_RETURN) {
    gen(node->lhs);
    printf("\tpop rax\n");

    printf("\tmov rsp, rbp\n");
    printf("\tpop rbp\n");
    printf("\tret\n");
    printf("\tpush rax\n");
    return;
  }
  if (node->ty == ND_COMP_STMT) {
    Vector *v = node->stmts;
    for (int i = 0; i < v->len; i++) {
      Node *n = (Node *)v->data[i];
      gen(n);
      //if (n->ty < ND_COMP_STMT) {
      printf("\tpop rax\n");
	//      }
    }
    printf("\tpush rax\n");
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
