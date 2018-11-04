enum {
  TK_NUM = 256,
  TK_EOF,
};

typedef struct {
  int ty; // type of token
  int val; // ty が TK_NUM の場合のその値
  char *input; // token string(for error message)
} Token;

Token tokens[];
