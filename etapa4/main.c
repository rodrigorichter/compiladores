#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "decompiler.h"
#include "semantic.h"

int isRunning();
int yyparse();
int yylex();
void yyerror(const char *s);
void initMe();
int semantic(AST *rootNode, map_t* scope);

extern FILE *yyin;
extern char *yytext;
extern int yylineno;


int main(int argc, char *argv[]) {
	if (argc != 3) exit(1);
	int token;

	initMe(); //function will be called by tests

	FILE *fp;
	fp = fopen(argv[1],"r");
	if (fp == NULL) {
		printf("Could not open input file: %s\n",argv[1]);
		exit(2);
	}

	FILE *output;
	output = fopen(argv[2],"w");
	if (output == NULL) {
		printf("Could not open output file: %s\n",argv[2]);
		exit(2);
	}
	setOutputFile(output);


	yyin = fp;

	int parseError = yyparse();
	if(parseError==1) {
		exit(3);
	}
	astPrint(root,0);

	int semanticResult = semantic(root, programScope);
	printf("result = %d\n", semanticResult);
	if( semanticResult != 0) {
		printf("Algum erro na verificacao semantica.\n");
	} else {
		printf("Sucesso na verificacao semantica.\n");
	}
	exit(semanticResult);
}