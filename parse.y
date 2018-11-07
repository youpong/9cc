終端記号を大文字.非終端記号を小文字で表す。
四則演算 with () ver.
expr: mul
expr: mul "+" expr
expr: mul "-" expr
mul: term
mul: term "*" mul
mul: term "/" mul
term: NUMBER
term: "(" expr ")"
