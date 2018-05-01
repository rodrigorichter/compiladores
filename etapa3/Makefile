#
# UFRGS - Compiladores B - Marcelo Johann - 2009/2 - Etapa 1
#
# Makefile for single compiler call
# All source files must be included from code embedded in scanner.l
# In our case, you probably need #include "hashmap.c" at the beginning
# and #include "main.c" in the last part of the scanner.l
#

etapa3: main.o y.tab.o lex.yy.o hashmap.o symbols.o ast.o decompiler.o
	gcc -o etapa3 main.o y.tab.o lex.yy.o symbols.o hashmap.o ast.o decompiler.o

main.o: main.c
	gcc -c main.c
hashmap.o: hashmap.c
	gcc -c hashmap.c
symbols.o: symbols.c
	gcc -c symbols.c
ast.o: ast.c
	gcc -c ast.c
decompiler.o: decompiler.c
	gcc -c decompiler.c

y.tab.o: y.tab.c
	gcc -c y.tab.c
y.tab.c: parser.y
	yacc parser.y -d

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c
lex.yy.c: scanner.l symbols.c hashmap.c 
	lex scanner.l

clean:
	rm *.o lex.yy.c y.tab.c y.tab.h etapa3