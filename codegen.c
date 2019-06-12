#include "9cc.h"
#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int label = 0;
static int rsp_cur;
static char *arg_rg[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9", NULL};

void gen_lval(Node *node) {
  if (node->ty != ND_IDENT) {
    error("変数ではありません\n");
  }

  SYM_REC *rec = query_var(node->name);
  if (rec == NULL) {
    error("未宣言の変数: %s\n", node->name);
  }

  printf("\tmov rax, rbp\n");
  printf("\tsub rax, %d\n", rec->addr);
  printf("\tpush rax\n");
}

void gen_func_def(Node *node) {
  int len;
  rsp_cur = 0;
  sym_tab = (SYM_TAB *)map_get(sym_tab->children, node->name);

  // print function name in label.
  printf("%s:\n", node->name);

  // print pro-logue
  printf("\tpush rbp\n");
  printf("\tmov rbp, rsp\n");
  printf("\tsub rsp, %d\n", sym_tab->var_cnt * 8);
  rsp_cur -= sym_tab->var_cnt * 8;

  // function params
  len = node->params->len;
  for (int i = 0; i < len; i++) {
    gen_lval((Node *)vec_at(node->params, i));
    printf("\tpush %s\n", arg_rg[i]);

    printf("\tpop rdi\n");
    printf("\tpop rax\n");
    printf("\tmov [rax], rdi\n");
  }

  // function body
  gen(node->body);

  // epi-logue
  printf("\tmov rsp, rbp\n");
  printf("\tpop rbp\n");
  printf("\tret\n");
  printf("\tpush rax\n");

  sym_tab = sym_tab->parent;
}

void gen_func_call(Node *node) {
  int len = node->args->len;

  for (int i = 0; i < len; i++) {
    if (arg_rg[i] == NULL)
      error("too match args");
    printf("\tpush %s\n", arg_rg[i]);
    rsp_cur -= 8;
  }
  for (int i = 0; i < len; i++) {
    gen((Node *)vec_at(node->args, i));
    printf("\tpop %s\n", arg_rg[i]);
  }

  int offset = 16 - abs(rsp_cur % 16);
  if (offset % 16 != 0)
    printf("\tsub rsp, %d\n", offset);

  printf("\tcall %s\n", node->name);
  if (offset % 16 != 0)
    printf("\tadd rsp, %d\n", offset);

  for (int i = 0; i < len; i++) {
    printf("\tpop %s\n", arg_rg[len - i - 1]);
    rsp_cur += 8;
  }

  printf("\tpush rax\n");
}

void gen_if(Node *node) {
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
}

void gen_while(Node *node) {
  // ラベルの作成 l
  node->label_head = malloc((3 + 1) * sizeof(char));
  node->label_tail = malloc((3 + 1) * sizeof(char));
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
}

void gen_return(Node *node) {
  gen(node->lhs);
  printf("\tpop rax\n");

  printf("\tmov rsp, rbp\n");
  printf("\tpop rbp\n");
  printf("\tret\n");
  printf("\tpush rax\n");
}
/**
 *
 */
void gen(Node *node) {
  switch (node->ty) {
  case ND_FUNC_CALL:
    gen_func_call(node);
    return;
  case ND_VAR_DEF:
    printf("\tpush rax\n");
    return;
  case ND_FUNC_DEF:
    gen_func_def(node);
    return;
  case ND_WHILE:
    gen_while(node);
    return;
  case ND_CONTINUE:
    printf("\tjmp %s\n", node->target->label_head);
    printf("\tpush rax\n");
    return;
  case ND_BREAK:
    printf("\tjmp %s\n", node->target->label_tail);
    printf("\tpush rax\n");
    return;
  case ND_IF:
    gen_if(node);
    return;
  case ND_RETURN:
    gen_return(node);
    return;
  case ND_COMP_STMT:
    for (int i = 0; i < node->stmts->len; i++) {
      gen((Node *)vec_at(node->stmts, i));
      printf("\tpop rax\n");
    }
    printf("\tpush rax\n");
    return;
  case ND_UNARY_MINUS:
    gen(node->lhs);
    printf("\tpop rax\n");
    printf("\tneg rax\n");
    printf("\tpush rax\n");
    return;
  case ND_NUM:
    printf("\tpush %d\n", node->val);
    return;
  case ND_IDENT:
    gen_lval(node);
    printf("\tpop rax\n");
    printf("\tmov rax, [rax]\n");
    printf("\tpush rax\n");
    return;
  case ND_ADDRESS_OF:
    gen_lval(node->lhs);
    printf("\tpop rax\n");
    printf("\tmov rax, [rax]\n");
    printf("\tpush rax\n");
    return;
  case ND_DEREFERENCE:
    gen_lval(node->lhs);
    printf("\tpop rax\n");
    printf("\tmov rax, [rax]\n");
    printf("\tpush rax\n");
    return;
  case '=':
    gen_lval(node->lhs);
    gen(node->rhs);

    printf("\tpop rdi\n");
    printf("\tpop rax\n");
    printf("\tmov [rax], rdi\n");
    printf("\tpush rdi\n");
    return;
  case '+':
  case '-':
  case '*':
  case '/':
  case ND_EQ:
  case ND_NE:
    // TODO:   if(node->lhs->eval_ty->ty == PTR)
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
    }
    printf("\tpush rax\n");
    return;
  default: 
    error("unexpected type of node\n");
  }
}
