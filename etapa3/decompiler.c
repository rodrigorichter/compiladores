#include "decompiler.h"

FILE *f;

void setOutputFile(FILE *output) {
	f = output;
}

void printType(AST *node);
void printDataType(AST *node);
void printIdentifier(AST *node);
void printSymbol(symbol_t *symbol);
void printAssignment();
void printVectorAssignment();
void printSemiColon();
void printComma();
void printLineFeed();
void printOpenIndex();
void printCloseIndex();
void printOpenParentheses();
void printCloseParentheses();
void printOpenBlock();
void printCloseBlock();
void printSpace();
void printPointerSymbol();
void printAddressSymbol();
void printInputKeyword();
void printOutputKeyword();
void printReturnKeyword();
void printIfKeyword();
void printElseKeyword();
void printThenKeyword();
void printWhileKeyword();
void printForKeyword();
void printToKeyword();

void decompile(AST *node) {
	if (!node){
		return;
	}

	switch(node->type) {
		
		case AST_PROGRAM:
			decompile(node->children[0]);//declaration list
			break;

		case AST_DEC_LIST:
			decompile(node->children[0]);//declaration of value, vector, initialized vector or function
			decompile(node->children[1]);//another declaration list
			break;

		case AST_DEC_VALUE:
			printDataType(node);
			printIdentifier(node);
			printAssignment();
			decompile(node->children[0]);//symbol
			printSemiColon();
			printLineFeed();
			break;

		case AST_DEC_VECTOR:
			printDataType(node);
			printIdentifier(node);
			printOpenIndex();
			decompile(node->children[0]);//size symbol
			printCloseIndex();
			printSemiColon();
			printLineFeed();
			break;

		case AST_DEC_VECTOR_INIT:
			printDataType(node);
			printIdentifier(node);
			printOpenIndex();
			decompile(node->children[0]);//size symbol
			printCloseIndex();
			printVectorAssignment();
			decompile(node->children[1]);//value list
			printSemiColon();
			printLineFeed();
			break;

		case AST_DEC_POINTER:
			printDataType(node);
			printPointerSymbol();
			printIdentifier(node);
			printAssignment();
			decompile(node->children[0]);//symbol
			printSemiColon();
			printLineFeed();
			break;

		case AST_DEC_FUNC:
			printDataType(node);
			printIdentifier(node);

			printOpenParentheses();
			decompile(node->children[0]);//parameter list
			printCloseParentheses();
			printSpace();
			
			decompile(node->children[1]);//block
			break;

		case AST_PARAM_LIST:
			decompile(node->children[0]);//parameter
			if(node->children[1]) {
				printComma();
				decompile(node->children[1]);//parameter list
			}
			break;

		case AST_PARAM:
			printDataType(node);
			printIdentifier(node);
			break;

		case AST_VALUE_LIST:
			//declaration of value, vector, initialized vector or function
			decompile(node->children[0]);
			printSpace();
			//another declaration list
			decompile(node->children[1]);
			break;

		case AST_SYMBOL:
			printSymbol(node->symbol);
			break;

		case AST_SYMBOL_ADDRESS:
			printAddressSymbol();
			printIdentifier(node);
			break;

		case AST_SYMBOL_POINTER:
			printPointerSymbol();
			printIdentifier(node);
			break;

		case AST_BLOCK: 
			printOpenBlock();
			printLineFeed();
			//command list
			decompile(node->children[0]);
			printCloseBlock();
			printLineFeed();
			break;

		case AST_CMD_LIST:
			//command
			decompile(node->children[0]);
			printSemiColon();
			printLineFeed();
			//outra lista de comandos
			decompile(node->children[1]);
			break;

		case AST_VALUE_ASS:
			printIdentifier(node);
			printAssignment();
			decompile(node->children[0]);//expression
			break;

		case AST_VECTOR_ASS:
			printIdentifier(node);
			printOpenIndex();
			decompile(node->children[0]);//index expression
			printCloseIndex();
			printAssignment();
			decompile(node->children[1]);//value expression
			break;

		case AST_VECTOR_ACCESS:
			printIdentifier(node);
			printOpenIndex();
			decompile(node->children[0]);//index expression
			printCloseIndex();
			break;

		case AST_READ:
			printInputKeyword();
			printIdentifier(node);
			break;

		case AST_PRINT:
			printOutputKeyword();
			decompile(node->children[0]);//element list
			break;

		case AST_ELEM_LIST:
			//element(symbol/literal string or expression)
			decompile(node->children[0]);
			printSpace();
			decompile(node->children[1]);//another element list
			break;

		case AST_RETURN:
			printReturnKeyword();
			decompile(node->children[0]);//element list
			break;

		case AST_IF_THEN:
			printIfKeyword();
			printOpenParentheses();
			decompile(node->children[0]);//condition expression
			printCloseParentheses();
			printThenKeyword();
			printLineFeed();
			decompile(node->children[1]);//command
			break;

		case AST_IF_THEN_ELSE:
			printIfKeyword();
			printOpenParentheses();
			decompile(node->children[0]);//condition expression
			printCloseParentheses();
			printThenKeyword();
			printLineFeed();
			decompile(node->children[1]);//command
			printElseKeyword();
			decompile(node->children[2]);//another command
			printLineFeed();
			break;

		case AST_WHILE:
			printWhileKeyword();
			printOpenParentheses();
			decompile(node->children[0]);//condition expression
			printCloseParentheses();
			printLineFeed();
			decompile(node->children[1]);//command
			break;

		case AST_FOR:
			printForKeyword();
			printOpenParentheses();
			decompile(node->children[0]);
			printToKeyword();
			decompile(node->children[1]);
			printCloseParentheses();
			printLineFeed();
			decompile(node->children[2]);
			break;

		case AST_INVOKE_FUNC:
			printIdentifier(node);
			printOpenParentheses();
			decompile(node->children[0]);//argument list
			printCloseParentheses();
			break;

		case AST_ARG_LIST:
			decompile(node->children[0]);//arg
			if(node->children[1]) {
				printComma();
				decompile(node->children[1]);//parameter list
			}
			break;

		case AST_NOT:
			printType(node);
			decompile(node->children[0]);
			break;

		case AST_ADD:
		case AST_SUB:
		case AST_MULT:
		case AST_DIV:
		case AST_LESS:
		case AST_GREATER:
		case AST_LESS_EQ:
		case AST_GREATER_EQ:
		case AST_EQ:
		case AST_NEQ:
		case AST_AND:
		case AST_OR:
			decompile(node->children[0]);//expression
			printType(node);
			decompile(node->children[1]);//expression
			break;

		default:
			fprintf(f, "Not implemented yet. ast-> %d\n",node->type);
			break;
	}
	return;
}

void printSymbol(symbol_t *symbol) {
	switch (symbol->type) {
		case LIT_INTEGER:
			fprintf(f, "%ld", symbol->intValue);
			break;
		case LIT_REAL:
			fprintf(f, "%.2f", symbol->doubleValue);
			break;
		case LIT_CHAR:
			fprintf(f, "'%c'", symbol->charValue);
			break;
		case LIT_STRING:
			fprintf(f, "%s", symbol->stringValue);
			break;
		case TK_IDENTIFIER:
			fprintf(f, "%s", symbol->key);
			break;
	}
	return;
}

void printToKeyword() {
	fprintf(f, " to ");
	return;
}

void printForKeyword() {
	fprintf(f, "for ");
	return;
}

void printIfKeyword() {
	fprintf(f, "if ");
	return;
}

void printElseKeyword() {
	fprintf(f, "else ");
	return;
}

void printThenKeyword() {
	fprintf(f, "then ");
	return;
}

void printWhileKeyword() {
	fprintf(f, "while ");
	return;
}

void printReturnKeyword() {
	fprintf(f, "return ");
	return;
}

void printOutputKeyword() {
	fprintf(f, "print ");
	return;
}

void printInputKeyword() {
	fprintf(f, "read ");
	return;
}

void printAddressSymbol() {
	fprintf(f, "&");
	return;
}

void printPointerSymbol() {
	fprintf(f, "#");
	return;
}

void printSpace() {
	fprintf(f, " ");
	return;
}

void printOpenIndex() {
	fprintf(f, "[");
	return;
}

void printCloseIndex() {
	fprintf(f, "]");
	return;
}

void printOpenParentheses() {
	fprintf(f, "(");
	return;
}

void printCloseParentheses() {
	fprintf(f, ")");
	return;
}

void printOpenBlock() {
	fprintf(f, "{");
	return;
}

void printCloseBlock() {
	fprintf(f, "}");
	return;
}

void printSemiColon() {
	fprintf(f, ";");
	return;
}

void printComma() {
	fprintf(f, ", ");
	return;
}

void printLineFeed() {
	fprintf(f, "\n");
	return;
}

void printVectorAssignment() {
	fprintf(f, " : ");
	return;
}

void printAssignment() {
	fprintf(f, " = ");
	return;
}

void printIdentifier(AST *node) {
	fprintf(f, "%s", node->symbol->stringValue);
	return;
}

void printDataType(AST *node) {
	fprintf(f, "%s ", tokenToString(node->dataType));
	return;
}

void printType(AST *node) {
	fprintf(f, " %s ", astTypeToString(node->type));
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
		case AST_DEC_VECTOR:
			return "vector";
			break;
		case AST_DEC_VALUE:
			return "assignment";
			break;
		case AST_CMD_LIST:
			return "command list";
			break;
		case AST_PROGRAM:
			return "program";
			break;
		case AST_SYMBOL:
			return "symbol";
			break;
		case AST_VALUE_LIST:
			return "value list";
			break;
		case AST_BLOCK:
			return "block";
			break;
		case AST_ADD:
			return "+";
			break;
		case AST_SUB:
			return "-";
			break;
		case AST_MULT:
			return "*";
			break;
		case AST_DIV:
			return "/";
			break;
		case AST_LESS:
			return "<";
			break;
		case AST_GREATER:
			return ">";
			break;
		case AST_LESS_EQ:
			return "<=";
			break;
		case AST_GREATER_EQ:
			return ">=";
			break;
		case AST_EQ:
			return "==";
			break;
		case AST_NEQ:
			return "!=";
			break;
		case AST_AND:
			return "&&";
			break;
		case AST_OR:
			return "||";
			break;
		case AST_NOT:
			return "!";
			break;

		default: 
			return "default";
	}
}