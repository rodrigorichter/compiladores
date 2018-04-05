#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int yyparse();
int yylex();
void yyerror(const char *s);
void initMe();

extern FILE *yyin;
extern char *yytext;
extern int yylineno;


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

	while( isRunning() ) {
		yyparse();
	}
}