%{
	#include <stdio.h>
%}

%%

"hello world" printf("flw");
. ;

%%



int main() {
	yylex();

	printf("ea");
}

int yywrap(void) {
	return 1;
}
