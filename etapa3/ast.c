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
		// fprintf(stderr, "no node. returning.\n");
		return;
	}
	for(i = 0; i < level; ++i) {
		fprintf(stderr, "   ");
	}
	fprintf(stderr, "AST(");
	switch(node->type)  {
		case AST_VECTOR:
			fprintf(stderr, "AST_VECTOR, %s)\n",node->symbol->key);
			break;
		case AST_PARAML:
			fprintf(stderr, "AST_PARAML)\n");
			break;
		case AST_PARAM:
			fprintf(stderr, "AST_PARAM,%s)\n",node->symbol->key);
			break;
		case AST_FUNC_DECL:
			fprintf(stderr, "AST_FUNC_DECL,%s)\n",node->symbol->key);
			break;
		case AST_VALUEL: 
			fprintf(stderr, "AST_VALUEL)\n");
			break;
		case AST_SYMBOL: 
			fprintf(stderr, "AST_SYMBOL,%s)\n",node->symbol->key);
			break;
		case AST_ADD:
			fprintf(stderr, "AST_ADD)\n");
			break;
		case AST_SUB:
			fprintf(stderr, "AST_SUB)\n");
			break;
		case AST_CMDL:
			fprintf(stderr, "AST_CMDL)\n");
			break;
		case AST_ASS:
			fprintf(stderr, "AST_ASS,%s)\n",node->symbol->key);
			break;
		case AST_PROGRAM:
			fprintf(stderr, "AST_PROGRAM)\n");
			break;
		case AST_BLOCK:
			fprintf(stderr, "AST_BLOCK)\n");
			break;
		case AST_FOR:
			fprintf(stderr, "AST_FOR)\n");
			break;
		case AST_PRINT:
			fprintf(stderr, "AST_PRINT)\n");
			break;
		case AST_ELEML:
			fprintf(stderr, "AST_ELEML)\n");
			break;
		default:
			fprintf(stderr, "UNRECOGNIZED AST TYPE)\n");
			break;	
	}
	for(i=0; i < MAX_CHILDREN; ++i) {
		astPrint(node->children[i], level + 1);
	}
}