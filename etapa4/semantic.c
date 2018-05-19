#include "semantic.h"
extern map_t scannedSymbolsMap;

int literalFromKeyword(int keyword) {
	switch (keyword) {
		case KW_INT: return LIT_INTEGER;
		case KW_CHAR: return LIT_CHAR;
		case KW_FLOAT: return LIT_REAL;
		default: exit(-1);
	}
}

int typeCanTakeLiteral(int type, int literal) {
	if(type == literal) {return 1;}//mega gambi hehe
	switch (type) {
		case KW_INT:
			return (literal == LIT_INTEGER);
		case KW_FLOAT:
			return (literal == LIT_REAL);
		case KW_CHAR:
			return (literal == LIT_CHAR || literal == LIT_INTEGER);
		default:
			return 0;
	}
}


int semantic(AST *node) {
	int error, type, literal, size, argCount;
	char *identifier;
	AST *valueNode, *listNode;
	symbol_t *declaredSymbol;

	if (!node){
		return SEMANTIC_SUCCESS;
	}
	switch(node->type) {
		
		case AST_PROGRAM:
			printf("semantic AST_PROGRAM OK.\n");
			return semantic(node->children[0]);//declaration list

		case AST_DEC_LIST:
			printf("semantic AST_DEC_LIST OK\n");
			error = semantic(node->children[0]);//declaration of value, vector, initialized vector or function
			if(error) {
				return error;
			}
			error = semantic(node->children[1]);//another declaration list
			if(error) {
				return error;
			}
			return SEMANTIC_SUCCESS;

		case AST_DEC_VALUE:
			printf("semantic AST_DEC_VALUE\n");
			
			identifier = node->symbol->key;
			type = node->dataType;

			valueNode = node->children[0];
			literal = valueNode->symbol->type;

			if ( ! typeCanTakeLiteral(type, literal) ) {
				printf("declarou um %d, mas forneceu um %d\n",type,literal);
				return SEMANTIC_ERROR;
			}

			declaredSymbol = getSymbol(declaredSymbolsMap, identifier);
			if(declaredSymbol) {
				printf("Simblo já declarado.\n");
				return SEMANTIC_ERROR;
			}

			printf("added to declared map: %s, as %d\n", identifier,literal);
			addSymbol(declaredSymbolsMap, literal, identifier, node->symbol->line);

			return SEMANTIC_SUCCESS;

		case AST_DEC_VECTOR:
			printf("semantic AST_DEC_VECTOR OK.\n");
			identifier = node->symbol->key;
			literal = literalFromKeyword(node->dataType);
			size = node->children[0]->symbol->intValue;
			
			declaredSymbol = getSymbol(declaredSymbolsMap, identifier);
			if(declaredSymbol) {
				printf("Simblo já declarado.\n");
				return SEMANTIC_ERROR;
			}

			printf("added to declared map: %s, as %d\n", identifier,literal);
			declaredSymbol = addSymbol(declaredSymbolsMap, literal, identifier, node->symbol->line);
			declaredSymbol->isArray = 1;
			declaredSymbol->arrayCapacity = size;
			return SEMANTIC_SUCCESS;

		case AST_DEC_VECTOR_INIT:
			printf("semantic AST_DEC_VECTOR_INIT\n");
			size = node->children[0]->symbol->intValue;
			type = node->dataType;
			identifier = node->symbol->stringValue;

			listNode = node->children[1];
			argCount = 0;
			while(listNode) {
				if (argCount > size - 1) {
					return SEMANTIC_ERROR;
				}

				literal = listNode->children[0]->symbol->type;
				if ( ! typeCanTakeLiteral(type, literal) ) {
					printf("declarou um %d, mas forneceu um %d\n",type,literal);
					return SEMANTIC_ERROR;
				}

				listNode = listNode->children[1];
				if(listNode == NULL && argCount < size - 1) {
					return SEMANTIC_ERROR;
				}
				argCount++;
			}

			declaredSymbol = getSymbol(declaredSymbolsMap, identifier);
			if(declaredSymbol) {
				printf("Simblo já declarado.\n");
				return SEMANTIC_ERROR;
			}
			literal = literalFromKeyword(type);
			printf("added to declared map: %s, as %d\n", identifier,literal);
			declaredSymbol = addSymbol(declaredSymbolsMap, literal, identifier, node->symbol->line);
			declaredSymbol->isArray = 1;
			declaredSymbol->arrayCapacity = size;
			return  SEMANTIC_SUCCESS;

		case AST_DEC_POINTER:
			printf("semantic AST_DEC_POINTER\n");
			identifier = node->symbol->stringValue;
			type = node->dataType;

			valueNode = node->children[0];
			literal = valueNode->symbol->type;

			if ( ! typeCanTakeLiteral(type, literal) ) {
				printf("declarou um %d, mas forneceu um %d\n",type,literal);
				return SEMANTIC_ERROR;
			}

			declaredSymbol = getSymbol(declaredSymbolsMap, identifier);
			if(declaredSymbol) {
				printf("Simblo já declarado.\n");
				return SEMANTIC_ERROR;
			}
			printf("added to declared map: %s, as %d\n", identifier,literal);
			declaredSymbol = addSymbol(declaredSymbolsMap, literal, identifier, node->symbol->line);
			declaredSymbol->isPointer = 1;
			return SEMANTIC_SUCCESS;

		case AST_DEC_FUNC:
			printf("semantic AST_DEC_FUNC\n");
			literal = node->dataType;
			identifier = node->symbol->stringValue;
			declaredSymbol = getSymbol(declaredSymbolsMap, identifier);
			if(declaredSymbol) {
				printf("Simblo já declarado.\n");
				return SEMANTIC_ERROR;
			}
			printf("added to declared map: %s, as %d\n", identifier,literal);
			declaredSymbol = addSymbol(declaredSymbolsMap, literal, identifier, node->symbol->line);
			declaredSymbol->isFunction = 1;
			
			listNode = node->children[0];//param_list
			argCount = 0;
			while(listNode) { //enquanto tem param_list
				valueNode = listNode->children[0];//pega param
				literal = valueNode->dataType; //pega tipo do param
				printf("parametro %d, %s, tipo %d\n",argCount, valueNode->symbol->key,literal);
				declaredSymbol->parametersIdentifiers[argCount] = valueNode->symbol->key;
				declaredSymbol->parametersTypes[argCount] = literal;
				argCount++;
				listNode = listNode->children[1];
			}
			return  SEMANTIC_SUCCESS;

		case AST_PARAM_LIST:
			printf("semantic AST_PARAM_LIST\n");
			semantic(node->children[0]);
			if(node->children[1]) {
				semantic(node->children[1]);
			}
			break;

		case AST_PARAM:
			printf("semantic AST_PARAM, %s\n", node->symbol->key);
			break;

		case AST_VALUE_LIST:
			printf("semantic AST_VALUE_LIST\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;

		case AST_SYMBOL:
			printf("semantic AST_SYMBOL, %s\n", node->symbol->key);
			break;

		case AST_TYPECHAR:
			printf("semantic AST_TYPECHAR, %s\n", node->symbol->key);
			break;

		case AST_TYPEINT:
			printf("semantic AST_TYPEINT, %s\n", node->symbol->key);
			break;

		case AST_TYPEFLOAT:
			printf("semantic AST_TYPEFLOAT, %s\n", node->symbol->key);
			break;

		case AST_SYMBOL_ADDRESS:
			printf("semantic AST_SYMBOL_ADDRESS, %s\n", node->symbol->key);
			break;

		case AST_SYMBOL_POINTER:
			printf("semantic AST_SYMBOL_POINTER, %s\n", node->symbol->key);
			break;

		case AST_BLOCK:
			printf("semantic AST_BLOCK\n");
			semantic(node->children[0]);
			break;

		case AST_CMD_LIST:
			printf("semantic AST_CMD_LIST\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;

		case AST_VALUE_ASS:
			printf("semantic AST_VALUE_ASS, %s\n", node->symbol->key);
			identifier = node->symbol->key;
			valueNode = node->children[0];
			literal = valueNode->symbol->type;
		
			printf("has to be declared: %s, as %d\n", identifier,literal);
			declaredSymbol = getSymbol(declaredSymbolsMap, identifier);
			if(!declaredSymbol) {
				printf("Simblo não declarado.\n");
				return SEMANTIC_ERROR;
			}
			type = declaredSymbol->type;
			printf("tipo: %d, lit: %d\n", type,literal);
			if ( ! typeCanTakeLiteral(type, literal) ) {
				printf("declarou um %d, mas forneceu um %d\n",type,literal);
				return SEMANTIC_ERROR;
			}
			return SEMANTIC_SUCCESS;

		case AST_VECTOR_ASS:
			printf("semantic AST_VECTOR_ASS, %s\n", node->symbol->key);
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;

		case AST_VECTOR_ACCESS:
			printf("semantic AST_VECTOR_ACCESS, %s\n", node->symbol->key);
			semantic(node->children[0]);
			break;

		case AST_READ:
			printf("semantic AST_READ, %s\n", node->symbol->key);
			semantic(node->children[0]);
			break;

		case AST_PRINT:
			printf("semantic AST_PRINT\n");
			semantic(node->children[0]);
			break;

		case AST_ELEM_LIST:
			printf("semantic AST_ELEM_LIST\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;

		case AST_RETURN:
			printf("semantic AST_RETURN\n");
			semantic(node->children[0]);
			break;

		case AST_IF_THEN:
			printf("semantic AST_IF_THEN\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;

		case AST_IF_THEN_ELSE:
			printf("semantic AST_IF_THEN_ELSE\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			semantic(node->children[2]);
			break;

		case AST_WHILE:
			printf("semantic AST_WHILE\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;

		case AST_FOR:
			printf("semantic AST_FOR\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			semantic(node->children[2]);
			break;

		case AST_INVOKE_FUNC:
			printf("semantic AST_INVOKE_FUNC, %s\n", node->symbol->key);
			semantic(node->children[0]);
			break;
			
		case AST_ARG_LIST:
			printf("semantic AST_ARG_LIST\n");
			semantic(node->children[0]);
			if(node->children[1]) {
				semantic(node->children[1]);
			}
			break;

		case AST_NOT:
			printf("semantic AST_NOT\n");
			semantic(node->children[0]);
			break;

		case AST_ADD:
			printf("semantic AST_ADD\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;
		case AST_SUB:
			printf("semantic AST_SUB\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;
		case AST_MULT:
			printf("semantic AST_MULT\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;
		case AST_DIV:
			printf("semantic AST_DIV\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;
		case AST_LESS:
			printf("semantic AST_LESS\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;
		case AST_GREATER:
			printf("semantic AST_GREATER\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;
		case AST_LESS_EQ:
			printf("semantic AST_LESS_EQ\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;
		case AST_GREATER_EQ:
			printf("semantic AST_GREATER_EQ\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;
		case AST_EQ:
			printf("semantic AST_EQ\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;
		case AST_NEQ:
			printf("semantic AST_NEQ\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;
		case AST_AND:
			printf("semantic AST_AND\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;
		case AST_OR:
			printf("semantic AST_OR\n");
			semantic(node->children[0]);
			semantic(node->children[1]);
			break;

		default:
			printf("semantic Not implemented yet. ast-> %d\n",node->type);
			break;
	}
	return 0;
}