#include <stdarg.h>
#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "parse.h"

Token *token;
char *user_input;

bool is_ascii_alphabet(char c){
	return 'a' <= c && c <= 'z';
}

void error_at(char *loc, char *fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	int pos = loc - user_input;
	fprintf(stderr, "%s\n", user_input);
	fprintf(stderr, "%*s", pos, " ");
	fprintf(stderr, "^ ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}
bool consume(char* op){
	if(token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len)){
		return false;
	}
	token = token->next;
	return true;
}

int consume_indent(char c){
	if(token->kind != TK_IDENT || !is_ascii_alphabet(c)){
		error_at(token->str, "%c is not indent", c);
	}
	token = token->next;
	return (c - 'a' + 1) * 8;
}

void expect(char* op){
	if(token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len)){
		error_at(token->str, "this is not '%c'", op);
	}
	token = token->next;
}

int expect_number() {
	if(token->kind != TK_NUM){
		error_at(token->str, "this is not number");
	}
	int val = token->val;
	token = token->next;
	return val;
}

int main(int argc, char **argv){
	if (argc != 2){
		fprintf(stderr, "the number of arg is wrong");
		return 1;
	}
	user_input = argv[1];
	token = tokenize(user_input);	
	program();

	printf(".intel_syntax noprefix\n");
	printf(".globl main\n");
	printf("main:\n");
	for(int i = 0; i < 100 && code[i] != NULL; i++){
		gen(code[i]);
	}

	printf("  pop rax\n");
	printf("  ret\n");
	return 0;
} 
