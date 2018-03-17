#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "hashmap.h"

#define KEY_MAX_LENGTH (256)
#define KEY_PREFIX ("somekey")
#define KEY_COUNT (1024*1024)

typedef struct symbol_s
{
    char key[KEY_MAX_LENGTH];
    int type;
    int line;
    int intValue;
    float floatValue;
    char charValue;
    char stringValue[KEY_MAX_LENGTH];
} symbol_t;


map_t symbolMap;

void printSymbol(symbol_t* symbol);
extern int initMap();
extern int addIntegerSymbol(int type, char* key, int line);
extern symbol_t* getSymbol(char *key);
// extern int addSymbol(int token, PFany f, any_t item);
// extern int getSymbol(int token, PFany f, any_t item);