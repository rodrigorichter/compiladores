#include <stdlib.h>
#include <stdio.h>
#include "symbols.h"

#ifndef AST_H_
#define AST_H_

#define MAX_CHILDREN 4

#define AST_SYMBOL 0
#define AST_ADD 1
#define AST_SUB 2
#define AST_CMDL 3
#define AST_ASS 4
#define AST_PROGRAM 5
#define AST_VALUEL 6
#define AST_FUNC_DECL 7
#define AST_PARAML 8
#define AST_PARAM 9
#define AST_BLOCK 10
#define AST_DEC 11

typedef struct AST_s
{
    int type;
    int returnType;
	symbol_t *symbol;
	struct AST_s *children[MAX_CHILDREN];
} AST;


AST *root;

extern AST* astCreate(int type, symbol_t* symbol, AST *firstChild, AST *secondChild, AST *thirdChild, AST *fourthChild);
extern void astPrint(AST *node, int level);
extern AST* astSetRoot(AST *newRoot);

#endif