/*
    Tim M. Lael
    CS4280
    p4
    25-APR 2017
*/
/*
scanner.h
header file file containing scanner function prototypes
*/
/* Begin inclusion-prevention mechanism */
#ifndef SCANNER_H
#define SCANNER_H
#include "token.h"
#define SPECIALCHARS 20		/* Number of special characters ascii mapped in allowable alphabet*/
#define TOKENS 25			/* Number of tokens defined - IDENT_tk has subset of keywords below */
#define KEYWORDS 10			/* Number of reserved keyword tokens defined (subset of identifiers) */
#define DFACOLS 24			/* Number of columns in DFA table */
#define DFAROWS 22			/* Number of rows in DFA table */
#define MAXIDLEN 8			/* Maximum allowable identifier/keyword length */
#define LOOKAHEAD 1			/* Number of lookahead characters (used to malloc string storing ascii char) */

/* Global token for scanner returns */
extern token_t returnTk;

/* 
Character mapping structure 
maps ascii value to a column of the state table
*/
typedef struct charMap{
	int input;
	int column;
}char_m;

/* 
State mapping structure 
maps state table state to a token
*/
typedef struct stateMap{
	int state;
	tok_id tkEnum;
}state_m;

/* Scanner function prototype */
token_t scan(FILE *infile, int *lineNo);
/* Token/Keyword mapping function prototype */
token_t lookup(int state, char *searchString);
/* Character mapping function prototype */
int matchValid(int currentChar);

/* End inclusion-prevention mechanism */
#endif /* SCANNER_H */
