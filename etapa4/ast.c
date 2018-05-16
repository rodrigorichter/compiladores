#include "ast.h"

AST* astSetRoot(AST *newRoot) {
	root = newRoot;
	return root;
}

AST* astCreate(int type, symbol_t* symbol, struct AST_s *firstChild, struct AST_s *secondChild, struct AST_s *thirdChild, struct AST_s *fourthChild) {
	AST *node;
	node = (AST*)calloc(1, sizeof(AST));
	node->type = type;
	node->symbol = symbol;
	node->children[0] = firstChild;
	node->children[1] = secondChild;
	node->children[2] = thirdChild;
	node->children[3] = fourthChild;
	return node;
}

void astPrint(AST *node, int level) {
	int i;
	if (!node){ 
		return;
	}
	for(i = 0; i < level; ++i) {
		printf("   ");
	}
	printf("AST(");
	switch(node->type) {
		
		case AST_PROGRAM:
			printf("AST_PROGRAM)\n");
			astPrint(node->children[0],level + 1);//declaration list
			break;

		case AST_DEC_LIST:
			printf("AST_DEC_LIST)\n");
			astPrint(node->children[0],level + 1);//declaration of value, vector, initialized vector or function
			astPrint(node->children[1],level + 1);//another declaration list
			break;

		case AST_DEC_VALUE:
			printf("AST_DEC_VALUE)\n");
			astPrint(node->children[0], level + 1);
			break;

		case AST_DEC_VECTOR:
			printf("AST_DEC_VECTOR)\n");
			astPrint(node->children[0], level + 1);
			break;

		case AST_DEC_VECTOR_INIT:
			printf("AST_DEC_VECTOR_INIT)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;

		case AST_DEC_POINTER:
			printf("AST_DEC_POINTER)\n");
			astPrint(node->children[0], level + 1);
			break;

		case AST_DEC_FUNC:
			printf("AST_DEC_FUNC, %s)\n", node->symbol->key);
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;

		case AST_PARAM_LIST:
			printf("AST_PARAM_LIST)\n");
			astPrint(node->children[0], level + 1);
			if(node->children[1]) {
				astPrint(node->children[1], level + 1);
			}
			break;

		case AST_PARAM:
			printf("AST_PARAM, %s)\n", node->symbol->key);
			break;

		case AST_VALUE_LIST:
			printf("AST_VALUE_LIST)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;

		case AST_SYMBOL:
			printf("AST_SYMBOL, %s)\n", node->symbol->key);
			break;

		case AST_SYMBOL_ADDRESS:
			printf("AST_SYMBOL_ADDRESS, %s)\n", node->symbol->key);
			break;

		case AST_SYMBOL_POINTER:
			printf("AST_SYMBOL_POINTER, %s)\n", node->symbol->key);
			break;

		case AST_BLOCK:
			printf("AST_BLOCK)\n");
			astPrint(node->children[0], level + 1);
			break;

		case AST_CMD_LIST:
			printf("AST_CMD_LIST)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;

		case AST_VALUE_ASS:
			printf("AST_VALUE_ASS, %s)\n", node->symbol->key);
			astPrint(node->children[0], level + 1);
			break;

		case AST_VECTOR_ASS:
			printf("AST_VECTOR_ASS, %s)\n", node->symbol->key);
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;

		case AST_VECTOR_ACCESS:
			printf("AST_VECTOR_ACCESS, %s)\n", node->symbol->key);
			astPrint(node->children[0], level + 1);
			break;

		case AST_READ:
			printf("AST_READ, %s)\n", node->symbol->key);
			astPrint(node->children[0], level + 1);
			break;

		case AST_PRINT:
			printf("AST_PRINT)\n");
			astPrint(node->children[0], level + 1);
			break;

		case AST_ELEM_LIST:
			printf("AST_ELEM_LIST)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;

		case AST_RETURN:
			printf("AST_RETURN)\n");
			astPrint(node->children[0], level + 1);
			break;

		case AST_IF_THEN:
			printf("AST_IF_THEN)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;

		case AST_IF_THEN_ELSE:
			printf("AST_IF_THEN_ELSE)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			astPrint(node->children[2], level + 1);
			break;

		case AST_WHILE:
			printf("AST_WHILE)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;

		case AST_FOR:
			printf("AST_FOR)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			astPrint(node->children[2], level + 1);
			break;

		case AST_INVOKE_FUNC:
			printf("AST_INVOKE_FUNC, %s)\n", node->symbol->key);
			astPrint(node->children[0], level + 1);
			break;
			
		case AST_ARG_LIST:
			printf("AST_ARG_LIST)\n");
			astPrint(node->children[0], level + 1);
			if(node->children[1]) {
				astPrint(node->children[1], level + 1);
			}
			break;

		case AST_NOT:
			printf("AST_NOT)\n");
			astPrint(node->children[0], level + 1);
			break;

		case AST_ADD:
			printf("AST_ADD)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;
		case AST_SUB:
			printf("AST_SUB)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;
		case AST_MULT:
			printf("AST_MULT)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;
		case AST_DIV:
			printf("AST_DIV)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;
		case AST_LESS:
			printf("AST_LESS)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;
		case AST_GREATER:
			printf("AST_GREATER)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;
		case AST_LESS_EQ:
			printf("AST_LESS_EQ)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;
		case AST_GREATER_EQ:
			printf("AST_GREATER_EQ)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;
		case AST_EQ:
			printf("AST_EQ)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;
		case AST_NEQ:
			printf("AST_NEQ)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;
		case AST_AND:
			printf("AST_AND)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;
		case AST_OR:
			printf("AST_OR)\n");
			astPrint(node->children[0], level + 1);
			astPrint(node->children[1], level + 1);
			break;

		default:
			printf("Not implemented yet. ast-> %d\n",node->type);
			break;
	}
	return;
}