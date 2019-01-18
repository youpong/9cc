#include "util.h"
#include <stdbool.h>
#include <stdio.h>

// Tokenize
enum {
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

extern Vector *tokens;
extern int pos;
void tokenize();

FILE *yyin;

// Abstract Syntax Node
enum {
  ND_NUM = 256, // 整数
  ND_IDENT,     // 識別子
  ND_FUNC_CALL, // function call
  ND_EQ,        // '=='
  ND_NE,        // '!='
  /* これより下の node は返り値を持たない */
  ND_COMP_STMT, // compound statement
  ND_IF,        // if statement
  ND_WHILE,     // while statement
  ND_BREAK,     // break statement
  ND_CONTINUE,  // continue statement
  ND_RETURN,    // return statement
  ND_FUNC_DEF,  // function definition
};

typedef struct Node Node;
struct Node {
  int ty; // 演算子 | ND_NUM | ND_IDENT
  Node *lhs;
  Node *rhs;
  int val;
  char *name;
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

extern Vector *code;

Node *new_node(int, Node *, Node *);
Node *new_node_num(int);
Node *new_node_id(char *name);

// Parse
void parse();

// Generate
void gen(Node *);

// Variables table
extern Map *var_tab;
extern int var_cnt;
typedef struct {
  int token;
  int addr;
} SYM_REC;

void insert(char *, int);
SYM_REC *lookup(char *);

// init.c
void init();

// debug.c
void p_tree(Node *);

// main.c
extern bool cmdln_flg;
extern bool ast_flg;
extern char **ARGV;
