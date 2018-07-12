#include "symbols.h"
#include "y.tab.h"
#include <string.h>

int initMaps() {
	scannedSymbolsMap = hashmap_new();
	programScope = hashmap_new();
	return 0;
}

map_t newMap() {
	map_t map = hashmap_new();
	return map;
}

symbol_t* addSymbol(map_t *map, int type, char* key, int line) {
	symbol_t* symbol;
	symbol = malloc(sizeof(symbol_t));
	snprintf(symbol->key, KEY_MAX_LENGTH, "%s", key);
	
	symbol->line = line;
	symbol->type = type;
	symbol->dataType = 0;
	symbol->isArray=0;
	symbol->isPointer=0;
	symbol->isFunction=0;

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
		case TK_IDENTIFIER:
			snprintf(symbol->stringValue, KEY_MAX_LENGTH, "%s", key);
			break;
	}

	int error;
	error = hashmap_put(map, symbol->key, symbol);
	assert(error==MAP_OK);
	return symbol;
}


symbol_t* getSymbol(map_t *map, char *key) {
	symbol_t* symbol;
	int error;
    error = hashmap_get(map, key, (void**)(&symbol));
    if (error == MAP_MISSING) {
		return NULL;
	}
    /* Make sure the value was both found and the correct number */
    assert(error==MAP_OK);
    // printSymbol(symbol);
    return symbol;
}

void printDebugSymbol(symbol_t* symbol) {
	printf("Printing Symbol\n");
	printf("	-Key: %s\n", symbol->key);
	printf("	-Type: %d\n", symbol->type);
	printf("	-DataType: %d\n", symbol->dataType);
	printf("	-Line: %d\n\n", symbol->line);
	return;
}

void printSymbols(map_t *map) {
	hashmap_print(map);
	return;
}

symbol_t* makeTemp(void) {
	static int serialNum = 0;
	static char buffer[64];

	snprintf(buffer, 64, "__xXxtempVarxXx123456TEMP(%d)__",serialNum++);
	return addSymbol(programScope, LIT_INTEGER, buffer, 0);
}

symbol_t* makeLabel(void) {
	static int serialNum = 0;
	static char buffer[64];

	snprintf(buffer, 64, "__xXxtempVarxXx123456LABEL(%d)__",serialNum++);
	return addSymbol(programScope, LIT_INTEGER, buffer, 0);
}