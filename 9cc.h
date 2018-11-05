enum {
  TK_NUM = 256,
  TK_EOF,
};

enum {
  ND_NUM = 256,
};

typedef struct {
  int ty; // type of token
  int val; // ty が TK_NUM の場合のその値
  char *input; // token string(for error message)
} Token;

typedef struct Node {
  int ty;
  struct Node *lhs;
  struct Node *rhs;
  int val;
} Node;

Node *new_node(int, Node *, Node *);
Node *new_node_num(int);

Node *parse();
void gen(Node *);

//void error(int);

extern Token tokens[];
extern int pos;

typedef struct {
  Node stack[100];
  Node *cur;
} Node_Ite;

Node_Ite *setup_ite(Node *);
Node *next_node(Node_Ite *);
Node *new_node(int, Node *, Node *);
Node *new_node_num(int);
