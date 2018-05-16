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
%type <node> Eleml
%type <node> Elem
%type <node> Argl

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
			$$ = astCreate(AST_DEC_LIST,0,$1,$2,0,0);
			} else {
				$$ = $2;
			}
	}
	| {$$ = 0;}
	;

dec:
	Type TK_IDENTIFIER '=' Value ';' {
		$$ = astCreate(AST_DEC_VALUE,$2,$4,0,0,0);
		$$->dataType = $1;
	}
	|
	Type TK_IDENTIFIER '[' LIT_INTEGER ']' ';' {
		AST *size = astCreate(AST_SYMBOL,$4,0,0,0,0);
		$$ = astCreate(AST_DEC_VECTOR,$2,size,0,0,0);
		$$->dataType = $1;
	}
	|
	Type TK_IDENTIFIER '[' LIT_INTEGER ']' ':' Valuel ';' {
		AST *size = astCreate(AST_SYMBOL,$4,0,0,0,0);
		$$ = astCreate(AST_DEC_VECTOR_INIT,$2, size,$7,0,0);
		$$->dataType = $1;
	}
	|
	Type '#' TK_IDENTIFIER '=' Value ';' {
		$$ = astCreate(AST_DEC_POINTER,$3, $5,0,0,0);
		$$->dataType = $1;
	}
	|
	Type TK_IDENTIFIER '(' Paraml ')' block {
		$$ = astCreate(AST_DEC_FUNC,$2 ,$4,$6,0,0);
		$$->dataType = $1;
	}
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
	Value Valuel { $$ = astCreate(AST_VALUE_LIST,0,$1,$2,0,0); }
	| {$$ = 0;}
	;

Paraml:
	Param ',' Paraml { 
		if($3 != 0) {
			$$ = astCreate(AST_PARAM_LIST,0, $1,$3,0,0);
		} else {
			$$ = $1;
		}
	}
	|
	Param {
		$$ = astCreate(AST_PARAM_LIST, 0, $1,0,0,0);
	}
	| { $$ = 0; }
	;

Param:
	Type TK_IDENTIFIER {
		$$ = astCreate(AST_PARAM,$2 ,0,0,0,0);
		$$->dataType = $1;
	}
	;

block:
	'{' cmdl '}' {$$ = astCreate(AST_BLOCK,0,$2,0,0,0);}
	;

cmdl:
	cmd ';' cmdl {
		if($1 != 0) {
			$$ = astCreate(AST_CMD_LIST,0,$1,$3,0,0);
		} else {
			$$ = $3;
		}
	}
	|
	cmd {
		if($1 != 0) {
			$$ = astCreate(AST_CMD_LIST,0,$1,0,0,0);
		}
	}
	;

cmd:
	block {$$ = $1;}
	|
	TK_IDENTIFIER '=' expr {$$ = astCreate(AST_VALUE_ASS,$1,$3,0,0,0);}
	|
	TK_IDENTIFIER '[' expr ']' '=' expr {$$ = astCreate(AST_VECTOR_ASS,$1, $3,$6,0,0);}
	|
	KW_READ TK_IDENTIFIER {$$ = astCreate(AST_READ,$2,0,0,0,0);}
	|
	KW_PRINT Eleml {$$ = astCreate(AST_PRINT,0,$2,0,0,0);}
	|
	KW_RETURN expr {$$ = astCreate(AST_RETURN,0,$2,0,0,0);}
	|
	KW_IF '(' expr ')' KW_THEN cmd {$$ = astCreate(AST_IF_THEN,0,$3,$6,0,0);}
	|
	KW_IF '(' expr ')' KW_THEN cmd KW_ELSE cmd {$$ = astCreate(AST_IF_THEN_ELSE,0,$3,$6,$8,0);}
	|
	KW_WHILE '(' expr ')' cmd {$$ = astCreate(AST_WHILE,0,$3,$5,0,0);}
	|
	KW_FOR '(' TK_IDENTIFIER '=' expr KW_TO expr ')' cmd {
		AST *ass = astCreate(AST_VALUE_ASS,$3,$5,0,0,0);
		$$ = astCreate(AST_FOR,0, ass,$7,$9,0);
	}
	| {$$ = 0;}
	;

Eleml:
	Elem Eleml {
		if($2 != 0) {
			$$ = astCreate(AST_ELEM_LIST,0,$1,$2,0,0);
		} else {
			$$ = astCreate(AST_ELEM_LIST,0,$1,0,0,0);
		}
	}
	| {$$ = 0;}
	;

Elem:
	LIT_STRING {$$ = astCreate(AST_SYMBOL, $1, 0,0,0,0);}
	|
	expr {$$ = $1;}
	;

expr:

	expr '+' expr {$$ = astCreate(AST_ADD,0,$1,$3,0,0);}
	|
	expr '-' expr {$$ = astCreate(AST_SUB,0,$1,$3,0,0);}
	|
	expr '*' expr {$$ = astCreate(AST_MULT,0,$1,$3,0,0);}
	|
	expr '/' expr {$$ = astCreate(AST_DIV,0,$1,$3,0,0);}
	|
	expr '<' expr {$$ = astCreate(AST_LESS,0,$1,$3,0,0);}
	|
	expr '>' expr {$$ = astCreate(AST_GREATER,0,$1,$3,0,0);}
	|
	expr OPERATOR_LE expr {$$ = astCreate(AST_LESS_EQ,0,$1,$3,0,0);}
	|
	expr OPERATOR_GE expr {$$ = astCreate(AST_GREATER_EQ,0,$1,$3,0,0);}
	|
	expr OPERATOR_EQ expr {$$ = astCreate(AST_EQ,0,$1,$3,0,0);}
	|
	expr OPERATOR_NE expr {$$ = astCreate(AST_NEQ,0,$1,$3,0,0);}
	|
	expr OPERATOR_AND expr {$$ = astCreate(AST_AND,0,$1,$3,0,0);}
	|
	expr OPERATOR_OR expr {$$ = astCreate(AST_OR,0,$1,$3,0,0);}
	|
	'!' expr {$$ = astCreate(AST_NOT,0,$2,0,0,0);}
	|
	'(' expr ')' {$$ = $2;}
	|
	TK_IDENTIFIER {$$ = astCreate(AST_SYMBOL,$1,0,0,0,0);}
	|
	TK_IDENTIFIER '[' expr ']' {$$ = astCreate(AST_VECTOR_ACCESS,$1,$3,0,0,0);}
	|
	'&' TK_IDENTIFIER {$$ = astCreate(AST_SYMBOL_ADDRESS,$2,0,0,0,0);}
	|
	'#' TK_IDENTIFIER {$$ = astCreate(AST_SYMBOL_POINTER,$2,0,0,0,0);}
	|
	Value {$$ = $1;}
	|
	TK_IDENTIFIER '(' Argl ')' {$$ = astCreate(AST_INVOKE_FUNC,$1,$3,0,0,0);}
	|
	TK_IDENTIFIER '(' ')' {$$ = astCreate(AST_INVOKE_FUNC,$1,0,0,0,0);}
	;

Argl:
	expr ',' Argl {
		$$ = astCreate(AST_ARG_LIST,0,$1,$3,0,0);
	}
	|
	expr {
		$$ = $1;
	}
	;


%%

int yyerror(char const *s) {
	printf("%s at line %d\n", s,getLineNumber());
	return 0;
}