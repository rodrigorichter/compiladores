%{
#include "ast.h"
#include "decompiler.h"
#include<stdio.h>
#include<stdlib.h>
extern int getLineNumber(void);
int yyerror(char const *s);
int yylex();
%}

%union {
	symbol_t *symbol;
	char *charValue;
	struct AST_s *node;
	int token;
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

%error-verbose
%token <symbol>TK_IDENTIFIER 280
%token <symbol>LIT_INTEGER   281
%token <symbol>LIT_REAL      282
%token <symbol>LIT_CHAR      285
%token <symbol>LIT_STRING    286
%token TOKEN_ERROR   290

%type <node> program
%type <node> dec
%type <node> decl
%type <node> Value
%type <node> Valuel
%type <node> block
%type <node> cmdl
%type <node> cmd
%type <node> expr
%type <node> Param
%type <node> Paraml

%type <token> Type

%left OPERATOR_OR
%left OPERATOR_AND
%left '!' OPERATOR_EQ OPERATOR_NE
%left '>' OPERATOR_GE
%left '<' OPERATOR_LE
%left '+' '-'
%left '*' '/'
%right '['']'
%right '('')'

%%

program:
	decl { 
		$$ = astCreate(AST_PROGRAM,0,$1,0,0,0);
		astPrint(astSetRoot($$),0);
		decompile($$);
	}
	;

decl:
	dec decl { 
		if($1 != 0) {
			$$ = astCreate(AST_CMDL,0,$1,$2,0,0);
			} else {
				$$ = $2;
			}
	}
	| {$$ = 0;}
	;

dec:
	Type TK_IDENTIFIER '=' Value ';' {
		$$ = astCreate(AST_DEC,$2,$4,0,0,0);
	}
	|
	Type TK_IDENTIFIER '[' LIT_INTEGER ']' ';' {$$ = 0;}
	|
	Type TK_IDENTIFIER '[' LIT_INTEGER ']' ':' Valuel ';' {
		$$ = astCreate(AST_DEC,$2,$7,0,0,0);
	}
	|
	Type '#' TK_IDENTIFIER '=' Value ';' {$$ = 0;}
	|
	Type TK_IDENTIFIER '(' Paraml ')' block {
		$$ = astCreate(AST_FUNC_DECL,$2,$4,$6,0,0);
		$$->returnType = $1;
	}
	|
	cmdl {$$ = 0;}
	|
	;

Type:
	KW_CHAR {$$ = KW_CHAR;}
	|
	KW_INT {$$ = KW_INT;}
	|
	KW_FLOAT {$$ = KW_FLOAT;}
	;

Value:
	LIT_INTEGER {
		$$ = astCreate(AST_SYMBOL,$1,0,0,0,0);
	}
	|
	LIT_REAL {
		$$ = astCreate(AST_SYMBOL,$1,0,0,0,0);
	}
	|
	LIT_CHAR {
		$$ = astCreate(AST_SYMBOL,$1,0,0,0,0);
	}
	;

Valuel:
	Value Valuel { $$ = astCreate(AST_VALUEL,0,$1,$2,0,0); }
	| {$$ = 0;}
	;

Paraml:
	Param ',' Paraml { 
		if($3 != 0) {
			$$ = astCreate(AST_PARAML,0,$1,$3,0,0);
		} else {
			$$ = $1;
		}
	}
	|
	Param {
		$$ = astCreate(AST_PARAML, 0, $1,0,0,0);
	}
	| { $$ = 0; }
	;

Param:
	Type TK_IDENTIFIER {
		$$ = astCreate(AST_SYMBOL,$2,0,0,0,0);
		$$->returnType = $1;
	}
	;

block:
	'{' cmdl '}' {$$ = astCreate(AST_BLOCK,0,$2,0,0,0);}
	;

cmdl:
	cmd ';' cmdl {
		if($1 != 0) {
			$$ = astCreate(AST_CMDL,0,$1,$3,0,0);
		} else {
			$$ = $3;
		}
	}
	|
	cmd {
		if($1 != 0) {
			$$ = astCreate(AST_CMDL,0,$1,0,0,0);
		}
	}
	;

cmd:
	block {$$ = $1;}
	|
	TK_IDENTIFIER '=' expr {$$ = astCreate(AST_ASS,$1,$3,0,0,0);}
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
	| {$$ = 0;}
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
	Value {$$ = $1;}
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
	return 0;
}