// Tokenize
enum {
  TK_NUM = 256,
  TK_EOF,
};

typedef struct {
  int ty; // type of token
  int val; // ty が TK_NUM の場合のその値
  char *input; // token string(for error message)
} Token;

extern Token tokens[];
extern int pos;

// Abstract Syntax Node
enum {
  ND_NUM = 256,
};

typedef struct Node {
  int ty;
  struct Node *lhs;
  struct Node *rhs;
  int val;
} Node;

Node *new_node(int, Node *, Node *);
Node *new_node_num(int);

// Parse
Node *parse();

// Generate
void gen(Node *);

// Utility
void error(char *, ...);
