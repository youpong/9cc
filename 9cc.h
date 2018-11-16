#include "util.h"

// Tokenize
enum {
  TK_NUM = 256, // integer
  TK_IDENT,     // 識別子
  TK_EQ,        // '=='
  TK_NE,        // '!='
  TK_EOF,       // end of file
};

typedef struct {
  int ty;      // type of token
  int val;     // ty が TK_NUM の場合のその値
  char *name;  // ty が TK_IDENT の場合その名前
  char *input; // token string(for error message)
} Token;

extern Vector *tokens;
extern int pos;
void tokenize(char *);

// Abstract Syntax Node
enum {
  ND_NUM = 256, // 整数
  ND_IDENT,     // 識別子
  ND_EQ,        // '=='
  ND_NE,        // '!='
};

typedef struct Node {
  int ty; // 演算子 | ND_NUM | ND_IDENT
  struct Node *lhs;
  struct Node *rhs;
  int val;
  char *name;
} Node;

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
