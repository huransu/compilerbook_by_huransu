typedef enum {
	ND_ADD,
	ND_SUB,
	ND_MUL,
	ND_DIV,
	ND_NUM,
	ND_GT,
	ND_GE,
	ND_LT,
	ND_LE,
	ND_EQ,
	ND_NE,
	ND_LVAR,
	ND_ASSIGN,
} NodeKind;

typedef struct Node Node;

struct Node {
	Node *lhs;
	Node *rhs;
	int val;
	int offset;
	NodeKind kind;
};

extern Node *code[100];

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *new_node_localvar(int offset);
Node *primary();
Node *unary();
Node *mul();
Node *add();
Node* relational();
Node *equality();
Node *assign();
Node *expr();
Node *stmt();
void program();
void gen(Node *node);
