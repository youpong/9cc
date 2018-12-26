#include "9cc.h"
void init() {
  insert("if", TK_IF);
  insert("else", TK_ELSE);
  insert("while", TK_WHILE);
  insert("break", TK_BREAK);
  insert("continue", TK_CONTINUE);
}
