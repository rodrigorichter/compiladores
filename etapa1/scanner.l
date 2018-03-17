%{
	#include <stdio.h>
	#include "tokens.h"
	#include "symbols.h"
	#include <string.h>
	int running = 1;
	extern int yylineno;
	extern void initMe();
	extern int getLineNumber(void);
%}

%option yylineno


%%

char return KW_CHAR;
int return KW_INT;
float return KW_FLOAT;
if return KW_IF;
then return KW_THEN;
else return KW_ELSE;
while return KW_WHILE;
for return KW_FOR;
read return KW_READ;
return return KW_RETURN;
print return KW_PRINT;

[a-zA-Z_][0-9a-zA-Z_]* return TK_IDENTIFIER;
\<\= return OPERATOR_LE;
\>\= return OPERATOR_GE;
\=\= return OPERATOR_EQ;
\!\= return OPERATOR_NE;
\&\& return OPERATOR_AND;
\|\| return OPERATOR_OR;

[0-9]+ {
	printf("LIT_INTEGER\n");
	addSymbol(LIT_INTEGER, yytext, yylineno);
	return LIT_INTEGER;
}

[0-9]+\.[0-9]+ {
	printf("LIT_REAL\n");
	addSymbol(LIT_REAL, yytext, yylineno);
	return LIT_REAL;
}

\'.\' {
	printf("LIT_CHAR\n");
	addSymbol(LIT_CHAR, yytext, yylineno);
	return LIT_CHAR;
}

\"([^"]|\\\"|\\n)*\" {
	printf("LIT_STRING\n");
	addSymbol(LIT_STRING, yytext, yylineno);
	return LIT_STRING;
}

[\-,;:()\[\]{}+\*\/<>=!&$#] return yytext[0];
\/\/.* yylineno++;
"/*"([^*]|\*+[^*/])*\*+"/" ;
[ \t]+ ;
"\r\n" ;
. return TOKEN_ERROR;

%%

int main(int argc, char *argv[]) {
	if (argc != 2) exit(1);
	int token;

	initMe(); //function will be called by tests

	FILE *fp;
	fp = fopen(argv[1],"r");
	if (fp == NULL) {
		printf("Could not open file: %s\n",argv[1]);
		exit(2);
	}
	yyin = fp;

	while( (token = yylex()) ) {
		printf("line: %d, token: %d, buffer: %s\n",yylineno,token,yytext);
	}
}

// Funções

void initMe() {
	printf("initMe()\n");
	running = 1;
	initMap();
}

int yywrap(void) {
	printf("yywrap EOF line: %d\n",yylineno);
	running = 0;

	char *key = "3";
	getSymbol(key);

	return 1;
}

int getLineNumber(void) {
	return yylineno;
}

int isRunning(void) {
	return running;
}