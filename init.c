#include "9cc.h"

void init() {
  sym_tab = append_sym_tab(NULL);

  entry_reserved("if", TK_IF);
  entry_reserved("else", TK_ELSE);
  entry_reserved("while", TK_WHILE);
  entry_reserved("break", TK_BREAK);
  entry_reserved("continue", TK_CONTINUE);
  entry_reserved("return", TK_RETURN);
  entry_reserved("int", TK_INT);
}
