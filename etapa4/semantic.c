#include "semantic.h"
#include "symbols.h"
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>

int verifyDeclarations(AST *rootNode) {
	if (rootNode==NULL)
		return 0;

	AST* node;
	node=rootNode->children[0];

	while (node->type==AST_DEC_LIST) { // enquanto nao termina a lista de declaracoes

		if (node->children[0]!=NULL) {
			if (node->children[0]->type==AST_DEC_VALUE) { // se a declaracao eh de uma variavel
				if (node->children[0]->symbol!=NULL) {
					if (node->children[0]->children[0]!=NULL) {
						if (node->children[0]->children[0]->symbol!=NULL) {
							if (node->children[0]->children[0]->symbol->type==LIT_INTEGER && node->children[0]->dataType==KW_INT) { // se a declaracao foi correta
								node->children[0]->symbol->dataType = DT_INTEGER;
								fprintf(stderr, "Variavel '%s' do tipo int declarada\n",node->children[0]->symbol->key);
							}
							else if (node->children[0]->children[0]->symbol->type==LIT_REAL && node->children[0]->dataType==KW_FLOAT) { // se a declaracao foi correta
								node->children[0]->symbol->dataType = DT_FLOAT;
								fprintf(stderr, "Variavel '%s' do tipo float declarada\n",node->children[0]->symbol->key);
							}
							else if (node->children[0]->children[0]->symbol->type==LIT_CHAR && node->children[0]->dataType==KW_CHAR) { // se a declaracao foi correta
								node->children[0]->symbol->dataType = DT_CHAR;
								fprintf(stderr, "Variavel '%s' do tipo char declarada\n",node->children[0]->symbol->key);
							}
							else {
								return 4;
							}
						}
					}
				}
			}
			else if (node->children[0]->type==AST_DEC_FUNC) { // se a declaracao eh de uma funcao
				if (node->children[0]->symbol!=NULL) {
					if (node->children[0]->dataType==KW_INT) {
						node->children[0]->symbol->dataType = DT_INTEGER;
						fprintf(stderr, "Funcao '%s' do tipo int declarada\n",node->children[0]->symbol->key);
					}
					else if (node->children[0]->dataType==KW_CHAR) {
						node->children[0]->symbol->dataType = DT_CHAR;
						fprintf(stderr, "Funcao '%s' do tipo char declarada\n",node->children[0]->symbol->key);
					}
					else if (node->children[0]->dataType==KW_FLOAT) {
						node->children[0]->symbol->dataType = DT_FLOAT;
						fprintf(stderr, "Funcao '%s' do tipo float declarada\n",node->children[0]->symbol->key);
					}
				}

			}
		}



		if(node->children[1]==NULL)
			break;
		else
			node=node->children[1];
	}

	


	return 0;
}