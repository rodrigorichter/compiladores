#
# UFRGS - Compiladores B - Marcelo Johann - 2009/2 - Etapa 1
#
# Makefile for single compiler call
# All source files must be included from code embedded in scanner.l
# In our case, you probably need #include "hashmap.c" at the beginning
# and #include "main.c" in the last part of the scanner.l
#

etapa2: main.o lex.yy.o y.tab.o hashmap.o symbols.o
	gcc -o etapa2 main.o lex.yy.o y.tab.o symbols.o hashmap.o

main.o: main.c
	gcc -c main.c
hashmap.o: hashmap.c
	gcc -c hashmap.c
symbols.o: symbols.c
	gcc -c symbols.c

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c
lex.yy.c: scanner.l symbols.c hashmap.c 
	lex scanner.l

y.tab.o: y.tab.c
	gcc -c y.tab.c
y.tab.c: parser.y
	yacc parser.y -d

clean:
	rm *.o lex.yy.c y.tab.c etapa2