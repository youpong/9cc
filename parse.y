
%{
  /*
   * 変数と四則演算 ver.
   *
   * 終端記号を大文字.非終端記号を小文字で表す。
   * 
   * ε は 空白で記述する。次のように記述した場合、
   * assign_ :   | "=" expr assign_
   * これは以下を意図している。
   * assign_ : ε | "=" expr assign_
   */
#include "lex.yy.h"  
#include "9cc.h"
%}

%union {
  int val;
  char *sym;
}

%token <sym> TK_IDENT
%token <val> TK_NUM

%%

program: assign program_
program_:   | assign program_

assign: expr assign_ ";"
assign_:   | "=" expr assign_
assign_:     "==" assign_
assign_:     "!=" assign_

expr: mul
expr: mul "+" expr
expr: mul "-" expr
mul: term
mul: term "*" mul
mul: term "/" mul
term: TK_NUM | TK_IDENT
term: "(" expr ")"

%%
