#include <stdbool.h> 

#ifndef TOKEN
#define TOKEN

typedef enum {
	TK_RESERVED,
	TK_IDENT,
	TK_NUM,
	TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
	TokenKind kind;
	Token *next;
	int val;
	char *str;
	int len;
};

#endif

bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);

