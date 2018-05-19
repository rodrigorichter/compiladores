#ifndef SYMBOLS_H_
#define SYMBOLS_H_
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "hashmap.h"


#define KEY_MAX_LENGTH (256)
#define KEY_PREFIX ("somekey")
#define KEY_COUNT (1024*1024)
#define MAX_PARAMS_COUNT 4

typedef struct symbol_s
{
    char key[KEY_MAX_LENGTH];
    int type;
    int dataType;
    int line;
    long int intValue;
    double doubleValue;
    char charValue;
    char stringValue[KEY_MAX_LENGTH];

    int isArray;
	int arrayCapacity;

	int isPointer;

	int isFunction;
	const char *parametersIdentifiers[MAX_PARAMS_COUNT];
	int parametersTypes[MAX_PARAMS_COUNT];
} symbol_t;


map_t scannedSymbolsMap;
map_t declaredSymbolsMap;

void printDebugSymbol(symbol_t* symbol);
extern int initMaps();
extern symbol_t* addSymbol(map_t *map, int type, char* key, int line);
extern symbol_t* getSymbol(map_t *map, char *key);

#endif