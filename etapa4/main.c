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

	int parseError=0;
	if( isRunning() ) {
		parseError=yyparse();
		if(parseError==1)
			exit(3);
		else if(parseError==0) {
			astPrint(root,0);
			verifyDeclarations(root);
			exit(0);
		}
	}
}