#
# UFRGS - Compiladores B - Marcelo Johann - 2009/2 - Etapa 1
#
# Makefile for single compiler call
# All source files must be included from code embedded in scanner.l
# In our case, you probably need #include "hashmap.c" at the beginning
# and #include "main.c" in the last part of the scanner.l
#

etapa5: hashmap.o y.tab.o symbols.o lex.yy.o ast.o decompiler.o semantic.o tac.o main.o 
	gcc -o etapa5  y.tab.o lex.yy.o symbols.o hashmap.o ast.o decompiler.o semantic.o tac.o main.o

main.o: main.c
	gcc -c main.c -Wall
hashmap.o: hashmap.c
	gcc -c hashmap.c -Wall
symbols.o: symbols.c
	gcc -c symbols.c -Wall
ast.o: ast.c
	gcc -c ast.c -Wall
decompiler.o: decompiler.c
	gcc -c decompiler.c -Wall
semantic.o: semantic.c
	gcc -c semantic.c -Wall

tac.o: tac.c
	gcc -c tac.c -Wall

y.tab.o: y.tab.c
	gcc -c y.tab.c -Wall
y.tab.c: parser.y
	yacc parser.y -d

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c -Wall
lex.yy.c: scanner.l symbols.c hashmap.c 
	lex scanner.l

clean:
	rm *.o lex.yy.c y.tab.c y.tab.h etapa5