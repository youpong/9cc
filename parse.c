#include <stdlib.h>
#include <stdio.h>
#include "9cc.h"

Node *expr();
Node *mul();
Node *term();

Node *parse() {
  return expr();
}

Node *expr() {
  Node *lhs = mul();
  char *ops = "+-";
  if (tokens[pos].ty == TK_EOF)
    return lhs;
  for(char *ptr = ops; *ptr != '\0'; ptr++) {
    if (tokens[pos].ty == *ptr) {
      pos++;
      return new_node(*ptr, lhs, expr());
    }
  }
  return lhs; // epsilon
}
  
Node *mul() {
  char *ops = "*/";
  Node *lhs = term();
  if (tokens[pos].ty == TK_EOF)
    return lhs;
  for (char *ptr = ops; *ptr != '\0'; ptr++) {
    if (tokens[pos].ty == *ptr) {
      pos++;
      return new_node(*ptr, lhs, mul());
    }
  }
  return lhs; // epsilon
}

Node *term() {
  if(tokens[pos].ty == TK_EOF)
    error("unexpected EOF");
  if (tokens[pos].ty == TK_NUM) 
    return new_node_num(tokens[pos++].val);
  if (tokens[pos].ty == '(') {
    pos++;
    Node *node = expr();
    if(tokens[pos].ty != ')')
      error("対応する閉じカッコがありません: %s\n", tokens[pos].input);
    pos++;
    return node;
  }
  error("unexpected token: %s\n", tokens[pos].input);
}
