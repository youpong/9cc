# 終端記号を大文字.非終端記号を小文字で表す。
# 変数と四則演算 ver.
program: assign program'
program': ε | assign program'

assign: expr assign' ";"
assign': ε | "=" expr assign'
assign':     "==" assign'
assign':     "!=" assign'

expr: mul
expr: mul "+" expr
expr: mul "-" expr
mul: term
mul: term "*" mul
mul: term "/" mul
term: NUMBER | IDENT
term: "(" expr ")"
