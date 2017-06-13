/*
    Tim M. Lael
    CS4280
    p1
    26-FEB 2017
*/
/*
scanner.h
header file file containing scanner function prototypes
*/
/* Begin inclusion-prevention mechanism */
#ifndef SCANNER_H
#define SCANNER_H
#include "token.h"
#define SPECIALCHARS 20
#define TOKENS 25
#define KEYWORDS 10

typedef struct charMap{
	int input;
	int column;
}char_m;

typedef struct stateMap{
	int state;
	tok_id tkEnum;
}state_m;

/* Scanner function */
token_t scan(FILE *infile, int *lineNo);
/* Token/Keyword mapping function */
token_t lookup(int state, char *searchString);
/* Character mapping function */
int matchValid(int currentChar);

/* End inclusion-prevention mechanism */
#endif /* SCANNER_H */
