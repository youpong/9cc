#include <stdlib.h>
#include <stdio.h>
#include "9cc.h"

Node *expr();
Node *mul();
Node *term();

Node *parse() {
  return expr();
}

/*
expr: mul | mul "+" expr | mul "-" expr
--
expr: mul ( "+" expr | "-" expr | ε )
*/
Node *expr() {
  Node *lhs = mul();

  if (tokens[pos].ty == '+') {
    pos++;
    return new_node('+', lhs, expr());
  }
  
  if (tokens[pos].ty == '-') {
    pos++;
    return new_node('-', lhs, expr());
  }

  return lhs; // epsilon
}

/*
mul: term | term "*" mul | term "/" mul
--
mul: term ( "*" mul | "/" mul | ε )
 */
Node *mul() {
  Node *lhs = term();

  if (tokens[pos].ty == '*') {
    pos++;
      return new_node('*', lhs, mul());
  }
  
  if (tokens[pos].ty == '/') {
    pos++;
    return new_node('/', lhs, mul());
  }

  return lhs; // epsilon
}

/*
term: NUMBER | "(" expr ")"
 */
Node *term() {
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
