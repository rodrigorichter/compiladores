#include "semantic.h"
extern map_t scannedSymbolsMap;

symbol_t* getSymbolInScopes(map_t firstScope, map_t secondScope, char* identifier) {
	symbol_t *symbol;
	symbol = getSymbol(firstScope, identifier);
	if(symbol) {
		return symbol;
	}
	symbol = getSymbol(secondScope, identifier);
	if(symbol) {
		return symbol;
	}
	return NULL;
}

int literalDataTypeFromKeyword(int keyword) {
	switch (keyword) {
		case KW_INT: return LIT_INTEGER;
		case KW_CHAR: return LIT_CHAR;
		case KW_FLOAT: return LIT_REAL;
		case LIT_INTEGER: return LIT_INTEGER;
		case LIT_CHAR: return LIT_CHAR;
		case LIT_REAL: return LIT_REAL;
		default: 
			printf("literalDataTypeFromKeyword não pode converter tipo (%d)\n", keyword);
			return 0;
	}
}

int typeCanTakeliteralDataType(int type, int literalDataType) {
	if(type == literalDataType) {return 1;}//mega gambi hehe
	type=literalDataTypeFromKeyword(type);
	switch (type) {
		case LIT_INTEGER:
		case LIT_REAL:
		case LIT_CHAR:
			return (literalDataType == LIT_INTEGER || literalDataType == LIT_CHAR || literalDataType == LIT_REAL);
		default:
			return 0;
	}
}

//retorna 0 se não for possível
int tryCoercion(int firstType, int secondType) {
	if(firstType == secondType) {return firstType;}//mega gambi hehe
	switch (firstType) {
		case LIT_INTEGER:
			if(secondType == LIT_REAL) {
				return secondType;
			}
			return 0;
		case LIT_REAL:
			if(secondType == LIT_INTEGER) {
				return firstType;
			}
			return 0;
		default:
			return 0;
	}
}



int semantic(AST *node, map_t* scope) {
	if(!scope) {
		return SEMANTIC_ERROR;
	}
	int error, type, literalDataType, size, argCount, rightSideType,leftSideType, coercionType;
	char *identifier;
	AST *valueNode, *listNode;
	symbol_t *declaredSymbol;

	if (!node){
		return SEMANTIC_SUCCESS;
	}
	switch(node->type) {
		
		case AST_PROGRAM:
			printf("semantic AST_PROGRAM OK.\n");
			return semantic(node->children[0], programScope);//declaration list

		case AST_DEC_LIST:
			printf("semantic AST_DEC_LIST OK\n");
			error = semantic(node->children[0], programScope);//declaration of value, vector, initialized vector or function
			if(error) {
				return error;
			}
			error = semantic(node->children[1], programScope);//another declaration list
			if(error) {
				return error;
			}
			return SEMANTIC_SUCCESS;

		case AST_DEC_VALUE:
			printf("semantic AST_DEC_VALUE\n");
			
			identifier = node->symbol->key;
			type = node->dataType;

			valueNode = node->children[0];
			literalDataType = valueNode->symbol->type;

			if(! typeCanTakeliteralDataType(type, literalDataType)) {
			// if ( literalDataTypeFromKeyword(type) != literalDataType) {
				printf("declarou um %d, mas forneceu um %d\n",type,literalDataType);
				return SEMANTIC_ERROR;
			}

			declaredSymbol = getSymbol(programScope, identifier);
			if(declaredSymbol) {
				printf("Simbolo já declarado.\n");
				return SEMANTIC_ERROR;
			}

			printf("added to declared map: %s, as %d\n", identifier,literalDataTypeFromKeyword(type));
			addSymbol(programScope, literalDataTypeFromKeyword(type), identifier, node->symbol->line);

			return SEMANTIC_SUCCESS;

		case AST_DEC_VECTOR:
			printf("semantic AST_DEC_VECTOR OK.\n");
			identifier = node->symbol->key;
			literalDataType = literalDataTypeFromKeyword(node->dataType);
			size = node->children[0]->symbol->intValue;
			
			declaredSymbol = getSymbol(programScope, identifier);
			if(declaredSymbol) {
				printf("Simblo já declarado.\n");
				return SEMANTIC_ERROR;
			}

			printf("added to declared map: %s, as %d\n", identifier,literalDataType);
			declaredSymbol = addSymbol(programScope, literalDataType, identifier, node->symbol->line);
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

				literalDataType = listNode->children[0]->symbol->type;
				if ( literalDataTypeFromKeyword(type) != literalDataType) {
					printf("declarou um %d, mas forneceu um %d\n",type,literalDataType);
					return SEMANTIC_ERROR;
				}

				listNode = listNode->children[1];
				if(listNode == NULL && argCount < size - 1) {
					return SEMANTIC_ERROR;
				}
				argCount++;
			}

			declaredSymbol = getSymbol(programScope, identifier);
			if(declaredSymbol) {
				printf("Simblo já declarado.\n");
				return SEMANTIC_ERROR;
			}
			literalDataType = literalDataTypeFromKeyword(type);
			printf("added to declared map: %s, as %d\n", identifier,literalDataType);
			declaredSymbol = addSymbol(programScope, literalDataType, identifier, node->symbol->line);
			declaredSymbol->isArray = 1;
			declaredSymbol->arrayCapacity = size;
			return  SEMANTIC_SUCCESS;

		case AST_DEC_POINTER:
			printf("semantic AST_DEC_POINTER\n");
			identifier = node->symbol->stringValue;
			type = node->dataType;

			valueNode = node->children[0];
			literalDataType = valueNode->symbol->type;

			if ( ! typeCanTakeliteralDataType(type, literalDataType) ) {
				printf("declarou um %d, mas forneceu um %d\n",type,literalDataType);
				return SEMANTIC_ERROR;
			}

			declaredSymbol = getSymbol(programScope, identifier);
			if(declaredSymbol) {
				printf("Simblo já declarado.\n");
				return SEMANTIC_ERROR;
			}
			printf("added to declared map: %s, as %d\n", identifier,literalDataType);
			declaredSymbol = addSymbol(programScope, literalDataType, identifier, node->symbol->line);
			declaredSymbol->isPointer = 1;
			return SEMANTIC_SUCCESS;

		case AST_DEC_FUNC:
			printf("semantic AST_DEC_FUNC\n");
			
			literalDataType = literalDataTypeFromKeyword(node->dataType);

			identifier = node->symbol->stringValue;
			declaredSymbol = getSymbol(programScope, identifier);
			if(declaredSymbol) {
				printf("Simblo já declarado.\n");
				return SEMANTIC_ERROR;
			}
			
			printf("added to declared map: %s, as %d\n", identifier,literalDataType);
			declaredSymbol = addSymbol(programScope, literalDataType, identifier, node->symbol->line);
			declaredSymbol->isFunction = 1;

			map_t functionScope = newMap();
			declaredSymbol->scope = functionScope;

			addSymbol(functionScope, literalDataType, "_return_type_", 0);

			listNode = node->children[0];//param_list
			argCount = 0;
			while(listNode) { //enquanto tem param_list
				valueNode = listNode->children[0];//pega param
				literalDataType = literalDataTypeFromKeyword(valueNode->dataType); //pega tipo do param
				identifier = valueNode->symbol->key;
				printf("parametro %d, %s, tipo %d\n",argCount, identifier,literalDataType);

				symbol_t *paramSymbol = getSymbol(functionScope, identifier);
				if(paramSymbol) {
					printf("Simbolo já declarado na funcao\n");
					return SEMANTIC_ERROR;
				}
				paramSymbol = addSymbol(functionScope, literalDataType, identifier, node->symbol->line);
				paramSymbol->dataType = literalDataType;
				declaredSymbol->parameters[argCount] = paramSymbol;
				
				argCount++;
				listNode = listNode->children[1];
			}
			declaredSymbol->argCount = argCount;
			return  semantic(node->children[1], functionScope);//passa escopo pro bloco

		case AST_SYMBOL_POINTER:
		case AST_SYMBOL_ADDRESS:
		case AST_SYMBOL:
			printf("semantic AST_SYMBOL, %s\n", node->symbol->key);
			type = node->symbol->type;
			if(type != TK_IDENTIFIER) {
				node->dataType = node->symbol->type;
				return SEMANTIC_SUCCESS;
			}
			identifier = node->symbol->key;
			// printf("%d\n", node->symbol->type);
			declaredSymbol = getSymbolInScopes(scope, programScope, identifier);
			if (!declaredSymbol) {
				printf("Simbolo não declarado\n");
				return SEMANTIC_ERROR;
			}
			node->dataType = declaredSymbol->type;
			printf("semantic AST_SYMBOL, %s, tipo %d\n", node->symbol->key, node->dataType);

			if (declaredSymbol->isArray) {
				printf("Erro: Vetor deve ser usado somente com indexacao\n");
				return SEMANTIC_ERROR;
			}

			if (declaredSymbol->isFunction) {
				printf("Erro: Funcao deve ser usada somente com argumentos\n");
				return SEMANTIC_ERROR;
			}

			return SEMANTIC_SUCCESS;

		// case AST_TYPECHAR://desnecessário?
		// 	printf("semantic AST_TYPECHAR, %s\n", node->symbol->key);
		// 	break;

		// case AST_TYPEINT://desnecessário?
		// 	printf("semantic AST_TYPEINT, %s\n", node->symbol->key);
		// 	break;

		// case AST_TYPEFLOAT://desnecessário?
		// 	printf("semantic AST_TYPEFLOAT, %s\n", node->symbol->key);
		// 	break;


		case AST_BLOCK:
			printf("semantic AST_BLOCK\n");
			return semantic(node->children[0], scope);

		case AST_CMD_LIST:
			printf("semantic AST_CMD_LIST\n");
			error = semantic(node->children[0], scope);
			if(error) {
				return error;
			}
			error = semantic(node->children[1], scope);
			if(error) {
				return error;
			}
			return SEMANTIC_SUCCESS;

		case AST_VALUE_ASS:
			printf("semantic AST_VALUE_ASS, %s\n", node->symbol->key);

			error = semantic(node->children[0], scope); //se tem sucesso, popula node->children[0]->dataType com o tipo resultante
			if(error) {
				return error;
			}
			
			identifier = node->symbol->key;
			valueNode = node->children[0];
			literalDataType = valueNode->dataType;
		
			printf("has to be declared: %s, as %d\n", identifier,literalDataType);
			declaredSymbol = getSymbolInScopes(scope,programScope,identifier);
			if(!declaredSymbol) {
				printf("não estava declarado em nenhum escopo\n");
				return SEMANTIC_ERROR;
			}
			
			type = declaredSymbol->type;
			printf("tipo: %d, lit: %d\n", type,literalDataType);
			if ( ! typeCanTakeliteralDataType(type, literalDataType) ) {
				printf("declarou um %d, mas forneceu um %d\n", type, literalDataType);
				return SEMANTIC_ERROR;
			}

			if(declaredSymbol->isArray) {
				printf("Erro: Vetor nao pode ser usado sem indexacao\n");
				return SEMANTIC_ERROR;
			}

			if(declaredSymbol->isFunction) {
				printf("Erro: Funcao deve ser usada somente com argumentos\n");
				return SEMANTIC_ERROR;
			}

			return SEMANTIC_SUCCESS;

		case AST_VECTOR_ASS:
			printf("semantic AST_VECTOR_ASS, %s\n", node->symbol->key);
			error = semantic(node->children[0], scope); //se tem sucesso, popula node->children[0]->dataType com o tipo resultante
			if(error) {
				return error;
			}

			identifier = node->symbol->key;
			valueNode = node->children[0];
			literalDataType = valueNode->dataType;

			if(literalDataType!=LIT_INTEGER) {
				printf("Vector index has to be integer(%d). It is %d.\n",LIT_INTEGER,literalDataType);
				return SEMANTIC_ERROR;
			}
			
			error = semantic(node->children[1], scope); //se tem sucesso, popula node->children[0]->dataType com o tipo resultante
			if(error) {
				return error;
			}

			valueNode = node->children[1];
			literalDataType = valueNode->dataType;
			printf("has to be declared: %s, as %d\n", identifier,literalDataType);
			declaredSymbol = getSymbolInScopes(scope,programScope,identifier);
			if(!declaredSymbol) {
				printf("não estava declarado em nenhum escopo\n");
				return SEMANTIC_ERROR;
			}
			
			type = declaredSymbol->type;
			printf("tipo: %d, lit: %d\n", type,literalDataType);
			if ( ! typeCanTakeliteralDataType(type, literalDataType) ) {
				printf("declarou um %d, mas forneceu um %d\n", type, literalDataType);
				return SEMANTIC_ERROR;
			}

			if(!declaredSymbol->isArray) {
				printf("Erro: Variavel nao pode ser indexada\n");
				return SEMANTIC_ERROR;
			}

			if(declaredSymbol->isFunction) {
				printf("Erro: Funcao nao pode ser indexada\n");
				return SEMANTIC_ERROR;
			}

			return SEMANTIC_SUCCESS;

			break;

		case AST_VECTOR_ACCESS:
			printf("semantic AST_VECTOR_ACCESS, %s\n", node->symbol->key);
			error = semantic(node->children[0], scope); //se tem sucesso, popula node->children[0]->dataType com o tipo resultante
			if(error) {
				return error;
			}
			valueNode = node->children[0];
			literalDataType = valueNode->dataType;
			
			if(literalDataType!=LIT_INTEGER) {
				printf("Vector index has to be integer(%d). It is %d.\n",LIT_INTEGER,literalDataType);
				return SEMANTIC_ERROR;
			}

			identifier = node->symbol->key;

			declaredSymbol = getSymbolInScopes(scope,programScope,identifier);
			if(!declaredSymbol) {
				printf("não estava declarado em nenhum escopo\n");
				return SEMANTIC_ERROR;
			}
			
			type = declaredSymbol->type;
			node->dataType=type;

			if(!declaredSymbol->isArray) {
				printf("Erro: Variavel nao pode ser indexada\n");
				return SEMANTIC_ERROR;
			}

			return SEMANTIC_SUCCESS;

		case AST_READ:
			printf("semantic AST_READ, %s\n", node->symbol->key);
			// semantic(node->children[0]);
			break;

		case AST_PRINT:
			printf("semantic AST_PRINT\n");
			error = semantic(node->children[0], scope);
			if(error) {
				return error;
			}

			return SEMANTIC_SUCCESS;

		case AST_ELEM_LIST:
			printf("semantic AST_ELEM_LIST\n");
			error = semantic(node->children[0], scope);
			if(error) {
				return error;
			}

			error = semantic(node->children[1], scope);
			if(error) {
				return error;
			}

			return SEMANTIC_SUCCESS;

		case AST_RETURN:
			printf("semantic AST_RETURN\n");
			error = semantic(node->children[0], scope);
			if(error) {
				return error;
			}
			symbol_t *returnSymbol = getSymbol(scope, "_return_type_");
			if(!returnSymbol) {
				printf("Não encontrou return symbol\n");
				return SEMANTIC_ERROR;
			}

			printf("esperado (%d), recebido (%d)\n", returnSymbol->type, node->children[0]->dataType);
			if(node->children[0]->dataType != returnSymbol->type) {
				printf("Retornou tipo errado\n");
				return SEMANTIC_ERROR;
			}
			return SEMANTIC_SUCCESS;

		case AST_IF_THEN:
			printf("semantic AST_IF_THEN\n");
			error = semantic(node->children[0], scope);
			if(error) {
				return error;
			}

			if (!(node->children[0]->dataType==LIT_INTEGER||node->children[0]->dataType==LIT_REAL)) {
				printf("Expressao de tipo invalido.\n");
				return SEMANTIC_ERROR;
			}
			node->dataType=node->children[0]->dataType;

			error = semantic(node->children[1], scope);
			if(error) {
				return error;
			}

			return SEMANTIC_SUCCESS;	

			break;

		case AST_IF_THEN_ELSE:
			printf("semantic AST_IF_THEN_ELSE\n");
			error = semantic(node->children[0], scope);
			if(error) {
				return error;
			}

			if (!(node->children[0]->dataType==LIT_INTEGER||node->children[0]->dataType==LIT_REAL)) {
				printf("Expressao de tipo invalido.\n");
				return SEMANTIC_ERROR;
			}
			node->dataType=node->children[0]->dataType;

			error = semantic(node->children[1], scope);
			if(error) {
				return error;
			}

			error = semantic(node->children[2], scope);
			if(error) {
				return error;
			}

			return SEMANTIC_SUCCESS;

			break;

		case AST_WHILE:
			printf("semantic AST_WHILE\n");
			error = semantic(node->children[0], scope);
			if(error) {
				return error;
			}

			if (!(node->children[0]->dataType==LIT_INTEGER||node->children[0]->dataType==LIT_REAL)) {
				printf("Expressao de tipo invalido.\n");
				return SEMANTIC_ERROR;
			}
			node->dataType=node->children[0]->dataType;

			error = semantic(node->children[1], scope);
			if(error) {
				return error;
			}

			return SEMANTIC_SUCCESS;

			break;

		case AST_FOR:
			printf("semantic AST_FOR\n");
			error = semantic(node->children[0], scope);
			if(error) {
				return error;
			}

			error = semantic(node->children[1], scope);
			if(error) {
				return error;
			}

			error = semantic(node->children[2], scope);
			if(error) {
				return error;
			}

			return SEMANTIC_SUCCESS;
			break;

		case AST_INVOKE_FUNC:
			printf("semantic AST_INVOKE_FUNC, %s\n", node->symbol->key);
			
			identifier = node->symbol->key;
			declaredSymbol = getSymbolInScopes(scope, programScope, identifier);
			if(!declaredSymbol) {
				printf("Simblo não declarado.\n");
				return SEMANTIC_ERROR;
			}
			node->dataType = declaredSymbol->type;
			if (!declaredSymbol->isFunction) {
				printf("Simbolo não é função\n");
				return SEMANTIC_ERROR;
			}

			listNode = node->children[0];//arg_list
			argCount = 0;
			while(listNode) { //enquanto tem arg_list
				if(argCount >= declaredSymbol->argCount) {
					printf("Argumentos demais.\n");
					return SEMANTIC_ERROR;
				}
				valueNode = listNode->children[0];//pega symbol
				error = semantic(valueNode, scope);
				if(error) {
					printf("Problema em um argumento\n");
					return error;
				}
				
				literalDataType = valueNode->dataType; //pega tipo do param
				
				symbol_t *param = (symbol_t*)declaredSymbol->parameters[argCount];
				printf("arg: (%d) - param (%d)",literalDataType,param->dataType);
				if(!typeCanTakeliteralDataType(param->dataType,literalDataType)) {
					printf("Argumento com tipo diferente do esperado.\n");
					return SEMANTIC_ERROR;
				}
				//tem que ser igual ao que a função espera

				// identifier = valueNode->symbol->key;
				// printf("parametro %d, %s, tipo %d\n",argCount, identifier,literalDataType);

				// symbol_t *paramSymbol = getSymbolInScopes(scope, programScope, identifier);
				// if(!paramSymbol) {
				// 	printf("Simbolo não declarado\n");
				// 	return SEMANTIC_ERROR;
				// }
				// paramSymbol = addSymbol(functionScope, literalDataType, identifier, node->symbol->line);
				// declaredSymbol->parameters[argCount] = paramSymbol;
				
				argCount++;
				listNode = listNode->children[1];
			}
			if(argCount < declaredSymbol->argCount) {
				printf("Faltaram argumentos.\n");
				return SEMANTIC_ERROR;
			}
			return SEMANTIC_SUCCESS;
			
		case AST_ARG_LIST:
			printf("semantic AST_ARG_LIST\n");
			// semantic(node->children[0]);
			if(node->children[1]) {
				// semantic(node->children[1]);
			}
			break;

		case AST_NOT:
		case AST_GREATER:
		case AST_LESS_EQ:
		case AST_GREATER_EQ:
		case AST_EQ:
		case AST_NEQ:
		case AST_AND:
		case AST_OR:
		case AST_LESS:
		case AST_DIV:
		case AST_MULT:
		case AST_SUB:
		case AST_ADD:
			printf("semantic AST_BINARY OPERATION\n");
			printf("first child type %d\n",node->children[0]->type);
			error = semantic(node->children[0], scope);
			if (error) {
				return SEMANTIC_ERROR;
			}
			if (! node->children[1] ) {
				node->dataType = node->children[0]->dataType;	
				return SEMANTIC_SUCCESS;
			}
			error = semantic(node->children[1], scope);
			if (error) {
				return SEMANTIC_ERROR;
			}

			//se chegar aqui tem dois filhos com data type preenchido
			leftSideType = node->children[0]->dataType;
			rightSideType = node->children[1]->dataType;

			coercionType = tryCoercion(leftSideType, rightSideType);
			if(!coercionType) {
				return SEMANTIC_ERROR;
			}
			node->dataType = coercionType;//falta fazer coercion e checks disso
			if(node->type!=AST_ADD) {
				type = node->children[0]->symbol->type;
				if(type == TK_IDENTIFIER) {
					identifier = node->children[0]->symbol->key;
					declaredSymbol = getSymbolInScopes(scope, programScope, identifier);
					if(declaredSymbol->isPointer) {
						printf("Erro: A unica operacao permitida com ponteiros eh soma\n");
						return SEMANTIC_ERROR;
					}
				}
				type = node->children[1]->symbol->type;
				if(type == TK_IDENTIFIER) {
					identifier = node->children[1]->symbol->key;
					declaredSymbol = getSymbolInScopes(scope, programScope, identifier);
					if(declaredSymbol->isPointer) {
						printf("Erro: A unica operacao permitida com ponteiros eh soma\n");
						return SEMANTIC_ERROR;
					}
				}
			}
				

			return SEMANTIC_SUCCESS;
			
		default:
			printf("semantic Not implemented yet. ast-> %d\n",node->type);
			return SEMANTIC_ERROR;
	}
	return SEMANTIC_SUCCESS;
}