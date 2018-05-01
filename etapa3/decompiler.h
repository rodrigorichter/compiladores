#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "tokens.h"

extern void decompile(AST *node);
const char * tokenToString(int token);
const char * astTypeToString(int type);