#include "util.h"
#include <stdbool.h>
#include <stdio.h>

/* typedef, enum, struct */
// Symbol table
typedef struct Type Type;
struct Type {
  enum { INT, PTR } ty;
  Type *ptr_to;
};

typedef struct SYM_TAB SYM_TAB;
struct SYM_TAB {
  // Map children の中の特別な要素として parent を加えることができる
  // けどそうしてない。最初にそう書いたから。何か別の理由が生まれるまで
  // このままにする。
  // --
  // Map body の中に children を含めたい場合、 SYM_REC 構造体のメンバ
  // に SYM_TAB を加えることとメンバ token の値でそれが予約語/変数名/
  // 関数名のいずれかであることを切り替えるようにできるだろう。
  // そうしていない理由は上の件と一緒。
  SYM_TAB *parent;
  Map *children;
  int var_cnt;
  Map *body;
};

typedef struct {
  int token;
  Type *ty;
  int addr;
} SYM_REC;

// Tokenize
enum token_type {
  TK_NUM = 256, // integer
  TK_IDENT,     // 識別子
  TK_EQ,        // '=='
  TK_NE,        // '!='
  TK_IF,        // "if"
  TK_ELSE,      // "else"
  TK_WHILE,     // "while"
  TK_BREAK,     // "break"
  TK_CONTINUE,  // "continue"
  TK_RETURN,    // "return"
  TK_INT,       // "int"
  TK_EOF,       // end of file
};

typedef struct {
  int ty;     // type of token
  int val;    // ty が TK_NUM の場合のその値
  char *name; // ty が TK_IDENT の場合その名前
} Token;

// Abstract Syntax Node
enum node_type {
  ND_EQ = 256,    // '=='
  ND_NE,          // '!='
  ND_UNARY_MINUS, // unary operator '-'
  ND_ADDRESS_OF,  // address-of operator '&'
  ND_DEREFERENCE, // pointer dereference '*'
  ND_NUM,         // 整数
  ND_IDENT,       // 識別子
  ND_FUNC_CALL,   // function call
  ND_COMP_STMT,   // compound statement
  ND_VAR_DEF,     // variable definition
  ND_IF,          // if statement
  ND_WHILE,       // while statement
  ND_BREAK,       // break statement
  ND_CONTINUE,    // continue statement
  ND_RETURN,      // return statement
  ND_FUNC_DEF,    // function definition
};

typedef struct Node Node;
struct Node {
  int ty; // node_type(ND_EQ, ND_NE, ... , ND_FUNC_DEF)

  Type *c_ty; // type of c

  char *name;
  int val;

  Node *lhs;     // Left Hand Side or unary operators operand.
  Node *rhs;     // Right Hand Side
  Vector *stmts; // compound statements

  Node *cond;
  Node *then;
  Node *els;
  Node *body;

  // For break/continue
  Node *target;
  char *label_head;
  char *label_tail;

  // Function definition
  Vector *params;

  // Function call
  Vector *args;
};

/* Functions */
void init();
Vector *tokenize();
Vector *parse(Vector *);
void sema(Vector *);
void gen_x86(Vector *);

// Node
Node *new_node(int, Node *, Node *);
Node *new_node_num(int);
Node *new_node_id(char *name);

// Symbol
SYM_TAB *append_sym_tab(char *);
void entry_reserved(char *, int);
void entry_var(char *, Type *);
SYM_REC *query_reserved(char *);
SYM_REC *query_var(char *);

// debug.c
void p_tree(Node *);

// symbol_test.c
void run_symbol_test();
void run_pointer_test();

/* Variables */
extern FILE *yyin;

extern SYM_TAB *sym_tab;

extern bool ast_flg;
extern char **ARGV;
