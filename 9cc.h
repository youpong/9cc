enum {
  TK_NUM = 256,
  TK_EOF,
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

Node *expr();
void gen(Node *);

//void error(int);

extern Token tokens[];
extern int pos;

