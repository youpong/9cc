9cc C compiler
===
低レイヤを知りたい人のためのCコンパイラ作成入門[*1]を
参考に作り始める。 ドラゴンブック[*2]を参考に作り続けている。
目標は self-hosting。

Build
-----
    $ make
    $ make check
    
Usage
-----
Command Line interface.

    program_file Specifiles program filename.
    -ast         Output AST(Abstract Syntax Tree). 
    -e stmt      Specifies stmt from command-line.    
    -test        Do test.

Sample
------

     $ 9cc main.c > main.s
     $ 9cc -ast main.c > main.dot
     $ dot -Tpng main.dot > main.png
     
     $ 9cc -e '9-5+2;'
     $ 9cc -ast -e '9-5+2;' > tmp.dot

     $ 9cc -test
    
# 実装済み

* 四則演算
* 論理演算(!=, ==)
* if, while 文
* break/continue 文
* コメント文(/* C style */)
* 引数[あり|なし]の関数定義
* 引数[あり|なし]の関数呼び出し
* ローカル変数

# 実装予定

* 論理演算(&&, ||, &, |)
* for, do while, switch
* コメント文(// C++ style)
* グローバル変数
* 配列
* ポインタ
* 単項演算子(-/+)
* ++/--
* char/int型
* 文字列リテラル
* 構造体
* extern
* #include

# 環境

* OS: Ubuntu 18.10
* ISA: x86_64

# 参考
1: Rui Ueyama[2018] 「低レイヤを知りたい人のためのCコンパイラ作成入門」(https://www.sigbus.info/compilerbook/)
2: A.V.Aho,R.Sethi and J.D.Ullman[1986] Compilers - Principles, Techniques, and Tools, Addison-Wesley
コンパイラⅠ - 原理・技法・ツール 原田賢一訳, サイエンス社(1990)
