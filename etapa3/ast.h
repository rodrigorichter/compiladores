#ifndef AST_H_
#define AST_H_
#include <stdlib.h>
#include <stdio.h>
#include "symbols.h"


#define MAX_CHILDREN 4

#define AST_SYMBOL 0
#define AST_DEC_VALUE 4
#define AST_PROGRAM 5
#define AST_VALUE_LIST 6
#define AST_VALUE_ASS 42
#define AST_DEC_LIST 39
#define AST_DEC_VECTOR 38
#define AST_DEC_VECTOR_INIT 40
#define AST_DEC_POINTER 41
#define AST_PARAM_LIST 8


#define AST_ADD 1
#define AST_SUB 2
#define AST_CMD_LIST 3



#define AST_DEC_FUNC 7

#define AST_PARAM 9
#define AST_BLOCK 10
#define AST_VECTOR_ASS 11
#define AST_READ 12
#define AST_PRINT 13
#define AST_RETURN 14
#define AST_IF_THEN 15
#define AST_IF_THEN_ELSE 16
#define AST_WHILE 17
#define AST_FOR 18
#define AST_ELEM_LIST 19
// #define AST_DEC_INIT 20
#define AST_ARG_LIST 21
#define AST_INVOKE_FUNC 22
#define AST_VECTOR_ACCESS 23
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
#define AST_SYMBOL_ADDRESS 35
#define AST_SYMBOL_POINTER 36
#define AST_DEC 37

typedef struct AST_s
{
    int type;
    int dataType;
    int arraySize;
	symbol_t *symbol;
	struct AST_s *children[MAX_CHILDREN];
} AST;


AST *root;

extern AST* astCreate(int type, symbol_t* symbol, AST *firstChild, AST *secondChild, AST *thirdChild, AST *fourthChild);
extern void astPrint(AST *node, int level);
extern AST* astSetRoot(AST *newRoot);

#endif