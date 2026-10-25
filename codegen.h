#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
} NodeKind;

typedef struct Node Node;

struct Node {
	Node *lhs;
	Node *rhs;
	int val;
	NodeKind kind;
};

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *primary();
Node *unary();
Node *mul();
Node *add();
Node* relational();
Node *equality();
Node *expr();
void gen(Node *node);
