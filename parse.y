終端記号を大文字.非終端記号を小文字で表す。
加減算 ver.
expr: NUMBER
expr: NUMBER "+" expr
expr: NUMBER "-" expr
--
四則演算 ver.
expr: mul
expr: mul "+" expr
expr: mul "-" expr
mul: NUMBER
mul: NUMBER "*" mul
mul: NUMBER "/" mul
--
四則演算 with () ver.
expr: mul
expr: mul "+" expr
expr: mul "-" expr
mul: term
mul: term "*" mul
mul: term "/" mul
term: NUMBER
term: "(" expr ")"
---
expr: mul expr'
expr': epsilon | "+" expr | "-" expr
---
expr: mul ( epsilon | "+" expr | "-" expr )
---
expr: mul ( "+" expr | "-" expr ) ?
---
mul: term ( epsilon | "*" mul | "/" mul )
---
term: number | "(" expr ")"

