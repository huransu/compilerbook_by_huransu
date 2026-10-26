#include "9cc.h"
#include "codegen.h"
#include "parse.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

Node *code[100];

Node *new_node(NodeKind kind, Node *lhs, Node *rhs){
	Node *node = calloc(1,sizeof(Node));
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	return node;
}

Node *new_node_num(int val){
	Node *node = calloc(1,sizeof(Node));
	node->kind = ND_NUM;
	node->val = val;
	return node;
}

Node *new_node_localvar(int offset){
	Node *node = calloc(1,sizeof(Node));
	node->kind = ND_LVAR;
	node->offset = offset;
	return node;
}

Node *primary() {
	if(consume("(")){
		Node *node = expr();
		expect(")");
		return node;
	}
	if(is_ascii_alphabet(token->str[0])) {
		int indent = consume_indent();
		if(indent){
			return new_node_localvar(indent);
		}
	}
	return new_node_num(expect_number());
}

Node *unary(){
	if(consume("+")){
		return primary();
	}
	if(consume("-")){
		return new_node(ND_SUB, new_node_num(0),primary());
	}
	return primary();
}

Node *mul() {
	Node *node = unary();
	for(;;){
		if(consume("*")){
			node = new_node(ND_MUL, node, unary());
		}
		else if(consume("/")){
			node = new_node(ND_DIV, node, unary());
		}
		else{
			return node;
		}
	}
}

Node *add() {
	Node *node = mul();

	for(;;){
		if(consume("+")){
			node = new_node(ND_ADD, node, mul());
		}
		else if(consume("-")){
			node = new_node(ND_SUB, node, mul());
		}
		else{
			return node;
		}
	}
}

Node* relational(){
	Node *node = add();
	for(;;){
		if(consume(">")){
			node = new_node(ND_GT, node, add());
		}
		else if(consume(">=")){
			node = new_node(ND_GE, node, add());
		}
		else if(consume("<")){
			node = new_node(ND_LT, node, add());
		}
		else if(consume("<=")){
			node = new_node(ND_LE, node, add());
		}
		else{
			return node;
		}
	}
	return node;
}

Node *equality() {
	Node *node = relational();

	for(;;){
		if(consume("==")){
			node = new_node(ND_EQ, node, relational());
		}
		else if(consume("!=")){
			node = new_node(ND_NE, node, relational());
		}
		else{
			return node;
		}
	}
	return node;
}

Node *assign() {
	Node *node = equality();
	if(consume("=")){
		node = new_node(ND_ASSIGN, node, assign());
	}
	return node;
}

Node *expr() {
	return assign();
}

Node *stmt() {
	Node *node = expr();
	expect(";");
	return node;
}

void program() {
	int i = 0;
	while (!at_eof()){
		code[i++] = stmt();
	}
	code[i] = NULL;
}

void gen(Node *node){
	if(node->kind == ND_NUM){
		printf("  push %d\n", node->val);
		return;
	}
	if(node->kind == ND_LVAR){
		printf("  push rbp\n");
		printf("mov rbp, rsp\n");
		printf("sub rsp, %d", node->offset);
		return;
	}

	gen(node->lhs);
	gen(node->rhs);
	printf("  pop rdi\n");
	printf("  pop rax\n");

	switch(node->kind){
		case ND_ADD:
			printf("  add rax, rdi\n");
			break;
		case ND_SUB:
			printf("  sub rax, rdi\n");
			break;
		case ND_MUL:
			printf("  imul rax, rdi\n");
			break;
		case ND_DIV:
			printf("  cqo\n");
			printf("  idiv rdi\n");
			break;
		case ND_EQ:
			printf("  cmp rax, rdi\n");
			printf("  sete al\n");
			printf("  movzb rax, al\n");
			break;
		case ND_NE:
			printf("  cmp rax, rdi\n");
			printf("  ietne al\n");
			printf("  movzb rax, al\n");
			break;
		case ND_LT:
			printf("  cmp rax, rdi\n");
			printf("  setl al\n");
			printf("  movzb rax, al\n");
			break;
		case ND_LE:
			printf("  cmp rax, rdi\n");
			printf("  setle al\n");
			printf("  movzb rax, al\n");
			break;
		case ND_GT:
			printf("  cmp rdi, rax\n");
			printf("  setl al\n");
			printf("  movzb rax, al\n");
			break;
		case ND_GE:
			printf("  cmp rdi, rax\n");
			printf("  setle al\n");
			printf("  movzb rax, al\n");
			break;
	}
	printf("  push rax\n");
}

