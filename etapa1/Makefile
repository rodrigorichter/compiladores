#
# UFRGS - Compiladores B - Marcelo Johann - 2009/2 - Etapa 1
#
# Makefile for single compiler call
# All source files must be included from code embedded in scanner.l
# In our case, you probably need #include "hash.c" at the beginning
# and #include "main.c" in the last part of the scanner.l
#

etapa1: main.c lex.yy.c
	gcc -o etapa1  main.c lex.yy.c symbols.c hashmap.c
lex.yy.c: scanner.l symbols.c hashmap.c 
	lex scanner.l
clean:
	rm lex.yy.c etapa1