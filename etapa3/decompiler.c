#include "decompiler.h"

void decompile(AST *node) {
	int token = 0;
	AST *current;
	if (!node){ 
		// fprintf(stderr, "can't decompile null node. returning.\n");
		return;
	}
	//fprintf(stderr, "DECOMPILING %s\n",astTypeToString(node->type));

	switch(node->type) {
		case AST_PROGRAM:
			decompile(node->children[0]);
			break;
		case AST_CMDL:
			decompile(node->children[0]);//primeiro comando
			decompile(node->children[1]);//outra lista de comandos
			break;
		case AST_DEC:
			if(node->children[0]->type == AST_VALUEL) {
				current = node->children[0];
				token = current->children[0]->symbol->type;
				fprintf(stderr, "%s ", tokenToString(token));
				fprintf(stderr, "%s =", node->symbol->stringValue);
				
				while(current != NULL) {
					fprintf(stderr, " %s", current->children[0]->symbol->key);
					current = current->children[1];
				}
				fprintf(stderr, ";\n");
			} else {
				token = node->children[0]->symbol->type;
				fprintf(stderr, "%s ", tokenToString(token));
				fprintf(stderr, "%s = ", node->symbol->stringValue);
				fprintf(stderr, "%s;\n", node->children[0]->symbol->key);
			}
			break;
		case AST_ASS:
			if(node->children[0]->type == AST_VALUEL) {
				current = node->children[0];
				fprintf(stderr, "%s =", node->symbol->stringValue);
				
				while(current != NULL) {
					fprintf(stderr, " %s", current->children[0]->symbol->key);
					current = current->children[1];
				}
				fprintf(stderr, ";\n");
			} else {
				fprintf(stderr, "%s = ", node->symbol->stringValue);
				fprintf(stderr, "%s;\n", node->children[0]->symbol->key);
			}
			break;
		case AST_FUNC_DECL:
			fprintf(stderr, "%s ", tokenToString(node->returnType));
			fprintf(stderr, "%s", node->symbol->stringValue);
			fprintf(stderr, "(");
			current = node->children[0];//param list
			int shouldAddComma = 0;
			while(current != NULL) {	
				if(shouldAddComma) {
					fprintf(stderr, ", ");
				}			
				fprintf(stderr, "%s ", tokenToString(current->children[0]->returnType));
				fprintf(stderr, "%s", current->children[0]->symbol->stringValue);
				current = current->children[1];//param list
				shouldAddComma = 1;
			}
			fprintf(stderr, ") ");
			current = node->children[1];//block
			decompile(current);
			break;
		case AST_BLOCK: 
			fprintf(stderr, "{\n");
			decompile(node->children[0]);//command list
			fprintf(stderr, "}\n");
			break;
		default:
			fprintf(stderr, "Not implemented yet.\n");
			break;
	}
	return;
}

const char * tokenToString(int token) {
	switch (token) {
		case KW_FLOAT:
		case LIT_REAL:
			return "float";
			break;
		case KW_CHAR:
		case LIT_CHAR:
			return "char";
			break;
		case KW_INT:
		case LIT_INTEGER:
			return "int";
			break;
		case LIT_STRING:
			return "string";
			break;
		
		default: 
			return "default";
	}
}

const char * astTypeToString(int type) {
	switch (type) {
		case AST_DEC:
			return "declaration";
			break;
		case AST_ASS:
			return "assignment";
			break;
		case AST_CMDL:
			return "command list";
			break;
		case AST_PROGRAM:
			return "program";
			break;
		case AST_SYMBOL:
			return "symbol";
			break;
		case AST_VALUEL:
			return "value list";
			break;
		case AST_BLOCK:
			return "block";
			break;
		default: 
			return "default";
	}
}