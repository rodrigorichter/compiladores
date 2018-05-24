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
	if(!tac) return;
	fprintf(stderr, "TAC(");

	switch(tac->type) {
		case TAC_DEC_VALUE:
			fprintf(stderr, "TAC_DEC_VALUE");
			break;
		case TAC_BEGIN_FUNC:
			fprintf(stderr, "TAC_BEGIN_FUNC");
			break;
		case TAC_DEC_LIST:
			fprintf(stderr, "TAC_DEC_LIST");
			break;
		case TAC_END_FUNC:
			fprintf(stderr, "TAC_END_FUNC");
			break;
		default:
			fprintf(stderr, "%d",tac->type);
			break;
	}

	if (tac->res) fprintf(stderr, ",%s",tac->res->key);
	else fprintf(stderr, ",0");
	if (tac->op1) fprintf(stderr, ",%s",tac->op1->key);
	else fprintf(stderr, ",0");
	if (tac->op2) fprintf(stderr, ",%s",tac->op2->key);
	else fprintf(stderr, ",0");
	fprintf(stderr, ")\n");
}

void tacPrintBack(TAC *tac) {
	if (!tac) return;

	tacPrintSingle(tac);
	tacPrintBack(tac->prev);
}

TAC* generateCode(AST* node) {
	if (!node) return 0;

	TAC* result=0;
	TAC* sonCode[MAX_CHILDREN];

	for(int i=0;i<MAX_CHILDREN;i++) {
		sonCode[i] = generateCode(node->children[i]);
	}

	switch(node->type) {
		case AST_SYMBOL:
			result = tacCreate(TAC_SYMBOL, node->symbol,0,0);
			break;
		case AST_DEC_VALUE:
			result = tacCreate(TAC_DEC_VALUE,node->symbol,0,0);
			break;
		case AST_DEC_FUNC:
			result = tacJoin(tacJoin(tacJoin(tacCreate(TAC_BEGIN_FUNC,node->symbol,0,0),sonCode[0]),sonCode[1]),tacCreate(TAC_END_FUNC,0,0,0));
			break;
		case AST_VALUE_ASS:
			result = tacCreate(TAC_VALUE_ASS,sonCode[0]->res,node->symbol,0);
			break;
		case AST_ADD:
			result = tacCreate(TAC_ADD, node->symbol,0,0);
			break;
		default:
			result = tacJoin(tacJoin(tacJoin(sonCode[0],sonCode[1]),sonCode[2]),sonCode[3]);
			break;
	}

	return result;

}

TAC* makeBinOp(int type, TAC* sonCode0, TAC* sonCode1) {
	TAC* newTac = tacCreate(TAC_ADD, makeTemp(), sonCode0?->sonCode0->res:0,sonCode1?->sonCode1->res:0);

	return tacJoin(tacJoin(sonCode0,sonCode1),newTac);
}