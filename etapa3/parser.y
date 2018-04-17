%{
#include "symbols.h"
#include<stdio.h>
#include<stdlib.h>

%}

%union {
	symbol_t*symbol;
}

%token KW_CHAR       256
%token KW_INT        257
%token KW_FLOAT      259
%token KW_IF         261
%token KW_THEN       262
%token KW_ELSE       263
%token KW_WHILE      264
%token KW_FOR        265
%token KW_TO         269
%token KW_READ       266
%token KW_RETURN     267
%token KW_PRINT      268

%token OPERATOR_LE   270
%token OPERATOR_GE   271
%token OPERATOR_EQ   272
%token OPERATOR_NE   273
%token OPERATOR_AND  274
%token OPERATOR_OR   275

%token <symbol>TK_IDENTIFIER 280
%token <symbol>TK_LIT_INTEGER 291
%token <symbol>TK_LIT_REAL 	 292
%token <symbol>TK_LIT_CHAR   293
%token <symbol>TK_LIT_STRING 294
%token LIT_INTEGER   281
%token LIT_REAL      282
%token LIT_CHAR      285
%token LIT_STRING    286
%token TOKEN_ERROR   290

%left '-' '+'
%left '*' '/'

%%

program:
	decl
	;

decl:
	dec decl
	|
	;

dec:
	Type TK_IDENTIFIER '=' Value ';'
	|
	Type TK_IDENTIFIER '[' LIT_INTEGER ']' ';'
	|
	Type TK_IDENTIFIER '[' LIT_INTEGER ']' ':' Valuel ';'
	|
	Type '#' TK_IDENTIFIER '=' Value ';'
	|
	Type TK_IDENTIFIER '(' Paraml ')' block
	|
	cmdl
	|
	;

Type:
	KW_CHAR
	|
	KW_INT
	|
	KW_FLOAT
	;

Value:
	LIT_INTEGER
	|
	LIT_REAL
	|
	LIT_CHAR
	;

Valuel:
	Value Valuel
	|
	;

Paraml:
	Param ',' Paraml
	|
	Param
	|
	;

Param:
	Type TK_IDENTIFIER
	;

block:
	'{' cmdl '}'
	;

cmdl:
	cmd ';' cmdl
	|
	cmd
	;

cmd:
	block
	|
	TK_IDENTIFIER '=' expr
	|
	TK_IDENTIFIER '[' expr ']' '=' expr
	|
	KW_READ TK_IDENTIFIER
	|
	KW_PRINT Eleml
	|
	KW_RETURN expr
	|
	KW_IF '(' expr ')' KW_THEN cmd
	|
	KW_IF '(' expr ')' KW_THEN cmd KW_ELSE cmd
	|
	KW_WHILE '(' expr ')' cmd
	|
	KW_FOR '(' TK_IDENTIFIER '=' expr KW_TO expr ')' cmd
	|
	;

Eleml:
	Elem Eleml
	|
	;

Elem:
	LIT_STRING
	|
	expr
	;

expr:

	expr '+' expr
	|
	expr '-' expr
	|
	expr '*' expr
	|
	expr '/' expr
	|
	expr '<' expr
	|
	expr '>' expr
	|
	expr OPERATOR_LE expr
	|
	expr OPERATOR_GE expr
	|
	expr OPERATOR_EQ expr
	|
	expr OPERATOR_NE expr
	|
	expr OPERATOR_AND expr
	|
	expr OPERATOR_OR expr
	|
	'!' expr
	|
	'(' expr ')'
	|
	TK_IDENTIFIER
	|
	TK_IDENTIFIER '[' expr ']'
	|
	'&' TK_IDENTIFIER
	|
	'#' TK_IDENTIFIER
	|
	Value
	|
	TK_IDENTIFIER '(' Argl ')'
	|
	TK_IDENTIFIER '(' ')'
	;

Argl:
	expr ',' Argl
	|
	expr
	;


%%

int yyerror(char const *s) {
	printf("%s at line %d\n", s,getLineNumber());
}