#include "symbols.h"
#include "tokens.h"
#include <string.h>

int initMap() {
	printf("initMap()\n");
	symbolMap = hashmap_new();
	return 0;
}

int addSymbol(int type, char* key, int line) {
	printf("addSymbol(%d, %s, %d)\n", type, key, line);
	symbol_t* symbol;
	symbol = malloc(sizeof(symbol_t));
	snprintf(symbol->key, KEY_MAX_LENGTH, "%s", key);
	
	symbol->line = line;
	symbol->type = type;

	char *ptr;

	switch(type) {
		case LIT_INTEGER:
			symbol->intValue = strtol(key,&ptr,10);
			break;
		case LIT_REAL:
			symbol->doubleValue = strtod(key,&ptr);
			break;
		case LIT_CHAR:
			symbol->charValue = key[1];
			break;
		case LIT_STRING:
			snprintf(symbol->stringValue, KEY_MAX_LENGTH, "%s", key);
			break;
	}

	printSymbol(symbol);

	int error;
	error = hashmap_put(symbolMap, symbol->key, symbol);
	assert(error==MAP_OK);
	return 0;
}


symbol_t* getSymbol(char *key) {
	printf("getSymbol(%s)\n",key);
	symbol_t* symbol;
	int error;
    error = hashmap_get(symbolMap, key, (void**)(&symbol));
    /* Make sure the value was both found and the correct number */
    assert(error==MAP_OK);
    printSymbol(symbol);
    return symbol;
}

void printSymbol(symbol_t* symbol) {
	printf("Print Symbol\n");
	printf("Key: %s\n", symbol->key);
	printf("Type: %d\n", symbol->type);
	printf("Line: %d\n", symbol->line);
	return;
}