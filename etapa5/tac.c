#include "tac.h"
#include <stdlib.h>
#include <stdio.h>

TAC* tacCreate(int type, symbol_t* res, symbol_t* op1, symbol_t* op2) {
	TAC* newTac;
	newTac = (TAC*) calloc(1,sizeof(TAC));

	newTac->type = type;
	newTac->res = res;
	newTac->op1 = op1;
	newTac->op2 = op2;
	newTac->prev = 0;
	newTac->next = 0;

	return newTac;
}

TAC* tacJoin(TAC* t1, TAC* t2) {
	TAC* aux = 0;
	if(!t1) return t2;
	if(!t2) return t1;

	for (aux = t2;aux->prev;aux=aux->prev) ;

	aux->prev = t1;
	return t2;
}

TAC* tacPrintSingle(TAC* tac) {
	if(!tac) return 0;
	fprintf(stderr, "TAC(");

	switch(tac->type) {
		case TAC_SYMBOL:
			fprintf(stderr, "TAC_SYMBOL\n");
			break;
		case TAC_PROGRAM:
			fprintf(stderr, "TAC_PROGRAM\n");
			break;
		case TAC_DEC_LIST:
			fprintf(stderr, "TAC_DEC_LIST\n");
			break;
	}
}

TAC* generateCode(AST* node) {
	fprintf(stderr, "----OI----\n");
	if (!node) return 0;

	TAC* result=0;
	TAC* sonCode[MAX_CHILDREN];

	for(int i=0;i<MAX_CHILDREN;i++) {
		sonCode[i] = generateCode(node->children[i]);
	}

	switch(node->type) {
		case AST_PROGRAM:
		fprintf(stderr, "WTF, %d\n",sonCode[0]->type);
			result = tacJoin(tacCreate(TAC_PROGRAM,0,0,0),sonCode[0]);
			break;
		case AST_SYMBOL:
			result = tacCreate(TAC_SYMBOL, node->symbol,0,0);
			break;
		case AST_ADD:
			result = tacCreate(TAC_ADD, node->symbol,0,0);
			break;
	}

	return result;

}
