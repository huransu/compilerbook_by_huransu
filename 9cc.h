#include <stdbool.h>
#include "parse.h"

extern Token *token;
extern char *user_input;

void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
void expect(char* op);
int expect_number();

