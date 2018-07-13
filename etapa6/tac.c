#include "tac.h"
#include <stdlib.h>
#include <stdio.h>

TAC* makeBinOp(int type, TAC* sonCode0, TAC* sonCode1);
TAC* makeIfThen(TAC* sonCode0, TAC* sonCode1);
TAC* makeIfThenElse(TAC* sonCode0, TAC* sonCode1, TAC* sonCode2);
TAC* makeWhile(TAC* sonCode0, TAC* sonCode1);
//TAC* makeInvokeFunc(TAC* sonCode0);

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
	if(tac->type==TAC_SYMBOL) return;
	fprintf(stderr, "TAC(");

	switch(tac->type) {
		case TAC_DEC_VALUE:	fprintf(stderr, "TAC_DEC_VALUE");	break;
		case TAC_BEGIN_FUNC:fprintf(stderr, "TAC_BEGIN_FUNC");	break;
		case TAC_DEC_LIST:	fprintf(stderr, "TAC_DEC_LIST");	break;
		case TAC_END_FUNC:	fprintf(stderr, "TAC_END_FUNC");	break;
		case TAC_VALUE_ASS:	fprintf(stderr, "TAC_VALUE_ASS");	break;

		case TAC_ADD:		fprintf(stderr, "TAC_ADD");			break;
		case TAC_SUB:		fprintf(stderr, "TAC_SUB");			break;
		case TAC_MULT:		fprintf(stderr, "TAC_MULT");		break;
		case TAC_DIV:		fprintf(stderr, "TAC_DIV");			break;

		case TAC_NOT:		fprintf(stderr, "TAC_NOT");			break;
		case TAC_OR:		fprintf(stderr, "TAC_OR");			break;
		case TAC_AND:		fprintf(stderr, "TAC_AND");			break;
		case TAC_NEQ:		fprintf(stderr, "TAC_NEQ");			break;
		case TAC_EQ:		fprintf(stderr, "TAC_EQ");			break;
		case TAC_GREATER_EQ:fprintf(stderr, "TAC_GREATER_EQ");	break;
		case TAC_LESS_EQ:	fprintf(stderr, "TAC_LESS_EQ");		break;
		case TAC_LESS:		fprintf(stderr, "TAC_LESS");		break;
		case TAC_GREATER:	fprintf(stderr, "TAC_GREATER");		break;

		case TAC_LABEL:		fprintf(stderr, "TAC_LABEL");		break;
		case TAC_IFZERO:	fprintf(stderr, "TAC_IFZERO");		break;
		case TAC_JUMP:		fprintf(stderr, "TAC_JUMP");		break;
		case TAC_FUNC_CALL:	fprintf(stderr, "TAC_FUNC_CALL");	break;

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

void tacPrintForward(TAC *tac) {
	if (!tac) return;

	tacPrintSingle(tac);
	tacPrintForward(tac->next);
}

TAC* tacReverse(TAC *last) {
	TAC* tac = 0;

	for(tac=last;tac->prev;tac=tac->prev) {
		tac->prev->next = tac;
	}

	return tac;
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
			result = tacCreate(TAC_DEC_VALUE,node->symbol,sonCode[0]?sonCode[0]->res:0,0);
			break;

		case AST_DEC_FUNC:
			result = tacJoin(tacJoin(tacJoin(tacCreate(TAC_BEGIN_FUNC,node->symbol,0,0),sonCode[0]),sonCode[1]),tacCreate(TAC_END_FUNC,node->symbol,0,0));
			break;

		case AST_VALUE_ASS:
			result = tacJoin(sonCode[0],tacCreate(TAC_VALUE_ASS,node->symbol,sonCode[0]?sonCode[0]->res:0,0));
			break;

		case AST_ADD: 	result = makeBinOp(TAC_ADD,sonCode[0],sonCode[1]);	break;
		case AST_SUB: 	result = makeBinOp(TAC_SUB,sonCode[0],sonCode[1]);	break;
		case AST_MULT:	result = makeBinOp(TAC_MULT,sonCode[0],sonCode[1]);	break;
		case AST_DIV:	result = makeBinOp(TAC_DIV,sonCode[0],sonCode[1]);	break;

		case AST_NOT:	result = makeBinOp(TAC_NOT,sonCode[0],sonCode[1]);	break;
		case AST_OR:	result = makeBinOp(TAC_OR,sonCode[0],sonCode[1]);	break;
		case AST_AND:	result = makeBinOp(TAC_AND,sonCode[0],sonCode[1]);	break;
		case AST_NEQ:	result = makeBinOp(TAC_NEQ,sonCode[0],sonCode[1]);	break;
		case AST_EQ:			result = makeBinOp(TAC_EQ,sonCode[0],sonCode[1]);			break;
		case AST_GREATER_EQ:	result = makeBinOp(TAC_GREATER_EQ,sonCode[0],sonCode[1]);	break;
		case AST_LESS_EQ:		result = makeBinOp(TAC_LESS_EQ,sonCode[0],sonCode[1]);		break;
		case AST_LESS:			result = makeBinOp(TAC_LESS,sonCode[0],sonCode[1]);			break;
		case AST_GREATER:		result = makeBinOp(TAC_GREATER,sonCode[0],sonCode[1]);		break;

		case AST_IF_THEN:		result = makeIfThen(sonCode[0],sonCode[1]);		break;
		case AST_IF_THEN_ELSE:	result = makeIfThenElse(sonCode[0],sonCode[1],sonCode[2]);		break;
		case AST_INVOKE_FUNC:	result = tacJoin(sonCode[0],tacCreate(TAC_FUNC_CALL,node->symbol,0,0));	break;
		//case AST_ARG_LIST:		result = tacJoin(tacCreate(TAC_ARG,node->symbol,0,0),sonCode[0]);	break;
		case AST_WHILE:			result = makeWhile(sonCode[0],sonCode[1]);	break;



		default:
			result = tacJoin(tacJoin(tacJoin(sonCode[0],sonCode[1]),sonCode[2]),sonCode[3]);
			break;
	}

	return result;

}

TAC* makeBinOp(int type, TAC* sonCode0, TAC* sonCode1) {
	TAC* newTac = tacCreate(type, makeTemp(), sonCode0?sonCode0->res:0,sonCode1?sonCode1->res:0);

	return tacJoin(tacJoin(sonCode0,sonCode1),newTac);
}

TAC* makeIfThen(TAC* sonCode0, TAC* sonCode1) {
	TAC* ifTac = 0;
	TAC* labelTac = 0;
	symbol_t* label = 0;

	label = makeLabel();

	ifTac = tacCreate(TAC_IFZERO,label,sonCode0?sonCode0->res:0,0);
	labelTac = tacCreate(TAC_LABEL,label,0,0);

	return tacJoin(tacJoin(tacJoin(sonCode0,ifTac),sonCode1),labelTac);
}

TAC* makeIfThenElse(TAC* sonCode0, TAC* sonCode1, TAC* sonCode2) {
	TAC* ifTac = 0;
	TAC* labelTac = 0;
	symbol_t* label = 0;

	label = makeLabel();

	ifTac = tacCreate(TAC_IFZERO,label,sonCode0?sonCode0->res:0,0);
	labelTac = tacCreate(TAC_LABEL,label,0,0);

	return tacJoin(tacJoin(tacJoin(tacJoin(sonCode0,ifTac),sonCode1),labelTac),sonCode2);
}

TAC* makeWhile(TAC* sonCode0, TAC* sonCode1) {
	TAC* ifTac = 0;
	TAC* labelTac = 0;
	TAC* labelTacWhile = 0;
	TAC* jumpTac = 0;
	symbol_t* label = 0;
	symbol_t* labelWhile = 0;

	label = makeLabel();
	labelWhile = makeLabel();

	ifTac = tacCreate(TAC_IFZERO,label,sonCode0?sonCode0->res:0,0);
	labelTac = tacCreate(TAC_LABEL,label,0,0);
	labelTacWhile = tacCreate(TAC_LABEL,labelWhile,0,0);
	jumpTac = tacCreate(TAC_JUMP,labelWhile,0,0);

	return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(labelTacWhile,sonCode0),ifTac),sonCode1),jumpTac),labelTac);
}

void makeAsbly(TAC* tac) {
		fprintf(f, ".text\n.globl	main\n.type	main, @function\nmain:\n	.cfi_startproc\n 	pushq	%%rbp\n 	movq	%%rsp, %%rbp\n");
	while(tac) {
		switch(tac->type) {
			case TAC_ADD: fprintf(f, "	movl	%s(%%rbp), %%eax \n 	add	%s(%%rbp), %%eax \n 	movl	%%eax, %s(%%rbp) \n",tac->op1->key,tac->op2->key,tac->res->key); break;
			case TAC_SUB: fprintf(f, "	movl	%s(%%rbp), %%eax \n 	subl	%s(%%rbp), %%eax \n 	movl	%%eax, %s(%%rbp) \n",tac->op1->key,tac->op2->key,tac->res->key); break;
			case TAC_MULT: fprintf(f, "	movl	%s(%%rbp), %%eax \n 	imull	%s(%%rbp), %%eax \n 	movl	%%eax, %s(%%rbp) \n",tac->op1->key,tac->op2->key,tac->res->key); break;
			case TAC_DIV: fprintf(f, "	movl	%s(%%rbp), %%eax \n 	cltd\n 	idivl	%s(%%rbp) \n 	movl	%%eax, %s(%%rbp) \n",tac->op2->key,tac->op1->key,tac->res->key); break;
			case TAC_DEC_VALUE: fprintf(f, "	movl	%s, %s(%%rbp)\n",tac->op1->key,tac->res->key); break;
			case TAC_VALUE_ASS: fprintf(f, "	movl	%s(%%rbp), %%eax\n 	movl	%%eax, %s(%%rbp) \n",tac->op1->key,tac->res->key); break;
			case TAC_GREATER: fprintf(f, "	movl	%s(%%rbp), %%eax\n 	cmpl	%s(%%rbp), %%eax \n 	movl	%%eax, %s(%%rbp)\n",tac->op1->key,tac->op2->key,tac->res->key); break;
			case TAC_LESS: fprintf(f, "	movl	%s(%%rbp), %%eax\n 	cmpl	%s(%%rbp), %%eax \n 	movl	%%eax, %s(%%rbp)\n",tac->op1->key,tac->op2->key,tac->res->key); break;
			case TAC_GREATER_EQ: fprintf(f, "	movl	%s(%%rbp), %%eax\n 	cmpl	%s(%%rbp), %%eax \n 	movl	%%eax, %s(%%rbp)\n",tac->op1->key,tac->op2->key,tac->res->key); break;
			case TAC_LESS_EQ: fprintf(f, "	movl	%s(%%rbp), %%eax\n 	cmpl	%s(%%rbp), %%eax \n 	movl	%%eax, %s(%%rbp)\n",tac->op1->key,tac->op2->key,tac->res->key); break;
			case TAC_NOT: fprintf(f, "	cmpl	$0, %s(%%rbp)\n	sete	%%al\n 	movzbl	%%al, %s(%%rbp)\n",tac->op1->key,tac->res->key); break;
			case TAC_IFZERO: fprintf(f, "	cmpl	$0, %s(%%rbp)\n 	je	%s\n",tac->op1->key,tac->res->key); break;
			case TAC_LABEL: fprintf(f, "%s:\n",tac->res->key); break;

		}

		tac=tac->next;
	}
		fprintf(f, "	movl	$0, %%eax\n 	popq	%%rbp\n 	ret\n 	.cfi_endproc\n");

}

/*TAC* makeInvokeFunc(TAC* sonCode0) {
	TAC* newTac = 0;
	
	newTac = 
	
}*/