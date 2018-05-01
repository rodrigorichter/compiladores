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
#define AST_INDEX_ASS 11
#define AST_READ 12
#define AST_PRINT 13
#define AST_RETURN 14
#define AST_IF_THEN 15
#define AST_IF_THEN_ELSE 16
#define AST_WHILE 17
#define AST_FOR 18
#define AST_ELEML 19
// #define AST_DEC_INIT 20
#define AST_ARGL 21
#define AST_FUNC_CALL 22
#define AST_INDEX_SYMBOL 23
#define AST_NOT 24
#define AST_OR 25
#define AST_AND 26
#define AST_NEQ 27
#define AST_EQ 28
#define AST_GREATER_EQ 29
#define AST_LESS_EQ 30
#define AST_LESS 31
#define AST_GREATER 32
#define AST_DIV 33
#define AST_MULT 34
#define AST_BIT_AND 35
#define AST_OCTOTHORPE 36
#define AST_DEC 37
#define AST_VECTOR 38

typedef struct AST_s
{
    int type;
    int returnType;
    int arraySize;
	symbol_t *symbol;
	struct AST_s *children[MAX_CHILDREN];
} AST;


AST *root;

extern AST* astCreate(int type, symbol_t* symbol, AST *firstChild, AST *secondChild, AST *thirdChild, AST *fourthChild);
extern void astPrint(AST *node, int level);
extern AST* astSetRoot(AST *newRoot);

#endif