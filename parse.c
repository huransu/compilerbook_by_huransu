#include <ctype.h>
#include <stdbool.h> 
#include <stdlib.h>
#include <string.h>
#include "9cc.h"

Token *new_token(TokenKind kind, Token *cur, char *str, int len){
	Token *tok = calloc(1,sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	tok->len = len;
	cur->next = tok;
	return tok;
}

Token *tokenize(char *p){
	Token head;
	head.next = NULL;
	Token *cur = &head;

	while(*p){
		if(isspace(*p)){
			p++;
			continue;
		}
		if(strncmp(p, "==", 2) == 0 || strncmp(p, "!=", 2) == 0 || strncmp(p, ">=", 2) == 0 || strncmp(p, "<=", 2 == 0)){
			cur = new_token(TK_RESERVED, cur, p, 2);
			p += 2;
			continue;
		}
		if(strncmp(p, "+", 1) == 0 || strncmp(p, "-", 1) == 0 || strncmp(p, "*", 1) == 0 || strncmp(p, "/", 1) == 0 || strncmp(p, "(", 1) == 0 || strncmp(p, ")", 1) == 0){
			cur = new_token(TK_RESERVED, cur, p++, 1);
			continue;
		}
		if(isdigit(*p)){
			cur = new_token(TK_NUM, cur, p, 1);
			cur->val = strtol(p, &p, 10);
			continue;
		}
		error_at(token->str, "cannot tokenize this");
	}

	new_token(TK_EOF, cur, p, 0);
	return head.next;
}

bool at_eof() {
	return token->kind == TK_EOF;
}
