#include "9cc.h"
#include <stdlib.h>

SYM_TAB *append_sym_tab(char *func_name) {
  SYM_TAB *child;

  child = malloc(sizeof(SYM_TAB));
  child->parent = sym_tab;
  child->children = new_map();
  child->var_cnt = 0;
  child->body = new_map();

  // root sym_tab
  if (sym_tab != NULL)
    map_put(sym_tab->children, func_name, child);

  return child;
}

void entry_reserved(char *name, int ty) {
  SYM_REC *rec = malloc(sizeof(SYM_REC));
  rec->token = ty;
  map_put(sym_tab->body, name, rec);
}

SYM_REC *query_reserved(char *name) {
  return (SYM_REC *)map_get(sym_tab->body, name);
}

/*
 * name - variable name
 */
void entry_var(char *name, Type *ty) {
  SYM_REC *rec = malloc(sizeof(SYM_REC));

  rec->addr = 8 * (sym_tab->var_cnt + 1);
  sym_tab->var_cnt++;
  rec->ty = ty;
  map_put(sym_tab->body, name, rec);
}

SYM_REC *query_var(char *name) {
  SYM_REC *rec = NULL;

  for (SYM_TAB *tab = sym_tab; tab != NULL; tab = sym_tab->parent) {
    rec = (SYM_REC *)map_get(sym_tab->body, name);
    if (rec != NULL)
      break;
  }

  return rec;
}
