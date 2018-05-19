#include "ast.h"
#include "symbols.h"
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>

#define SEMANTIC_SUCCESS 0
#define SEMANTIC_ERROR 4

int semantic(AST *rootNode);