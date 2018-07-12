#ifndef TAC_HEADER
#define TAC_HEADER

#include "ast.h"
#include "symbols.h"
#include "y.tab.h"
#include <stdio.h>
#include "decompiler.h"
#include <stdlib.h>

#define TAC_SYMBOL 		1
#define TAC_PROGRAM 	2
#define TAC_DEC_LIST 	3
#define TAC_DEC_VALUE 	4
#define TAC_BEGIN_FUNC 	5
#define TAC_END_FUNC 	6
#define TAC_VALUE_ASS	7
#define TAC_ADD 		8
#define TAC_SUB 		9
#define TAC_MULT 		10
#define TAC_DIV 		11
#define TAC_NOT			12
#define TAC_OR			13
#define TAC_AND			14
#define TAC_NEQ			15
#define TAC_EQ			16
#define TAC_GREATER_EQ	17
#define TAC_LESS_EQ		18	
#define TAC_LESS		19	
#define TAC_GREATER		20	
#define TAC_LABEL		21	
#define TAC_IFZERO		22	
#define TAC_IF_THEN		23	
#define TAC_IF_THEN_ELSE	24	
#define TAC_JUMP 		25
#define TAC_FUNC_CALL 	26
#define TAC_ARG 	27

typedef struct tac {
	int type;
	symbol_t* res;
	symbol_t* op1;
	symbol_t* op2;
	struct tac *prev;
	struct tac *next;
} TAC;

void tacPrintBack(TAC* tac);
TAC* tacReverse(TAC *last);
void tacPrintForward(TAC* tac);
TAC* generateCode(AST* node);
void makeAsbly(TAC* tac);


#endif TAC_HEADER