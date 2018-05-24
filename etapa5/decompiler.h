#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "y.tab.h"

extern void decompile(AST *node);
extern void setOutputFile(FILE *output);
const char * tokenToString(int token);
const char * astTypeToString(int type);