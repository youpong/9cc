#include "9cc.h"
#include <stdlib.h>

void insert(char *name, int ty) {
  SYM_REC *rec = malloc(sizeof(SYM_REC));
  rec->token = ty;
  if (ty == TK_IDENT)
    rec->addr = var_cnt++;
  map_put(var_tab, name, rec);
}

SYM_REC *lookup(char *name) {
  return (SYM_REC *)map_get(var_tab, name);
}
