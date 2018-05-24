#ifndef TAC_HEADER
#define TAC_HEADER

#include "ast.h"
#include "symbols.h"
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>

#define TAC_SYMBOL 1
#define TAC_ADD 2
#define TAC_PROGRAM 3
#define TAC_DEC_LIST 4

typedef struct tac {
	int type;
	symbol_t* res;
	symbol_t* op1;
	symbol_t* op2;
	struct tac *prev;
	struct tac *next;
} TAC;

TAC* tacPrintSingle(TAC* tac);


#endif TAC_HEADER