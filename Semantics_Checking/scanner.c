/*
    Tim M. Lael
    CS4280
    p3
    14-APR 2017
*/
/*
scanner.c
source file containing scanner functions
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
#include "token.h"

token_t returnTk;

/* Lookup Structures */
/*************************************************************************************************/
/* 
   	DFA Table
   	transient states are low integers 0 - 21
   	errors are negative integers
   	final states are high integers 500 - 522
	final states are mapped to tokens below
*/
int DFAStateTable[DFAROWS][DFACOLS] = {
/*  L    D    =    <    >    :    +    -    *    /    &    %    .    (    )    ,    {    }    ;    [    ]    !   WS  EOF */
 {  1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  -2,   0,  -1}, // 0
 {  1, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500}, // 1
 {501,   2, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501}, // 2
 {502, 502, 503, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 504, 502, 502}, // 3
 { -2,  -2,  -2, 505,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2}, // 4
 { -2,  -2,  -2,  -2, 506,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2}, // 5
 {507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507}, // 6
 {508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508}, // 7
 {509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509}, // 8
 {510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510}, // 9
 {511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511}, // 10
 {512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512}, // 11
 {513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513}, // 12
 {514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514}, // 13
 {515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515}, // 14
 {516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516}, // 15
 {517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517}, // 16
 {518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518}, // 17
 {519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519}, // 18
 {520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520}, // 19
 {521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521}, // 20
 {522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522}  // 21
};

/* 
Array holding charMap structs for mapping special characters {ascii value, column} 
20 special characters defined as a constant in token.h (SPECIALCHARS)
*/
char_m specialChars[SPECIALCHARS] = {
	{61, 2}, {60, 3}, {62, 4}, {58, 5}, {43, 6},		// =, <, >, :, +
	{45, 7}, {42, 8}, {47, 9}, {38, 10}, {37, 11},		// -, *, /, &, %
	{46, 12}, {40, 13}, {41, 14}, {44, 15}, {123, 16},	// ., (, ), ,, {
	{125, 17}, {59, 18}, {91, 19}, {93, 20}, {33, 21}	// }, ;, [, ], !
};

/*
Final state to tokenMap struct mapping array 
25 final states defined as a constant in token.h (TOKENS)
*/
state_m finalStateMap[TOKENS] = {
  {-1, EOF_tk}, {-2, ERROR_tk},
  {500, IDENT_tk}, {501, NUM_tk},
  {502, EQUAL_tk}, {503, DBLEQUAL_tk}, {504, EQUALBANG_tk},
  {505, DBLLARROW_tk}, {506, DBLRARROW_tk},
  {507, COLON_tk},
  {508, PLUS_tk}, {509, MINUS_tk}, {510, ASTERISK_tk}, {511, SLASH_tk}, {512, AMP_tk} , {513, PCT_tk},
  {514, PERIOD_tk},
  {515, LPAREN_tk}, {516, RPAREN_tk}, {517, COMMA_tk}, {518, LBRACE_tk}, {519, RBRACE_tk},
  {520, SCOLON_tk}, {521, LBRACKET_tk}, {522, RBRACKET_tk}
};

/*
Table used to lookup keywords when an identifier is encountered 
10 keywords defined as a constant in token.h (KEYWORDS) 
*/
token_t keywordLookupTable[KEYWORDS] = {
{START_tk, "start", 0}, {STOP_tk, "stop", 0}, {DECISION_tk, "decision", 0},
{WHILE_tk, "while", 0}, {VOID_tk, "void", 0}, {INT_tk, "int", 0},
{RETURN_tk, "return", 0}, {READ_tk, "read", 0}, {PRINT_tk, "print", 0},
{PROGRAM_tk, "program", 0}
};
/* End Lookup Structures */
/*************************************************************************************************/


/* Mapping Functions */
/*************************************************************************************************/
/*
Character mapping function which uses specialChars array of charMap structs
to match those values. Letters, numbers and EOF are matched on the fly.
*/
int matchValid(int currentChar) {
	/* try to match characters */
	if (isalpha(currentChar)) {
		return 0;
	}
	/* try to match numbers */
	else if (isdigit(currentChar)) {
		return 1;
	}
	/* try to match WS */
	else if (isspace(currentChar)) {
		return 22;
	}
	/* try to match EOF */
	else if (currentChar == EOF) {
		return 23;
	}
	else if(currentChar != EOF || !isspace(currentChar) || !isdigit(currentChar) || !isalpha(currentChar)){ 
		int l;
		/* loop through special characters looking for a match */
		for (l = 0; l < SPECIALCHARS; l++) {
			if(specialChars[l].input == currentChar){
				return specialChars[l].column;
			}
		}
	}
	return -2;
}

/* Lookup function to map CurrentWord to tokens */
token_t lookup(int state, char *searchString) {
	int i;
	for (i = 0; i < TOKENS; i++) {
		if (finalStateMap[i].state == state) {
			returnTk.tkIdentifier = finalStateMap[i].tkEnum;
			returnTk.tkString = searchString;
			returnTk.lineNo = 999; /* 999 is a default/testing value */
		}
	}
	/*
	State indicates this an identifier. Use keyword lookup
	table to ensure it is not a keyword. If it is, prepare
	the identifier as a keyword and return it.
	*/
	if (state == 500) {
		for (i = 0; i < KEYWORDS; i++) {
			if (strcmp(keywordLookupTable[i].tkString, searchString) == 0) {
				returnTk.tkIdentifier = keywordLookupTable[i].tkIdentifier;
				returnTk.tkString = keywordLookupTable[i].tkString;
				returnTk.lineNo = 999; /* 999 is a default/testing value */
			}
		}
	}
return returnTk;
}
/* End Mapping Functions */
/*************************************************************************************************/


/* Scanner Function */
/*************************************************************************************************/
token_t scan(FILE *infile, int *lineNo) {
	/* DFA state implementation */
	int currentState = 0;
	int nextState = 0;

	/* initialize return token values to defaults */
	returnTk.tkIdentifier = 999;
	returnTk.tkString = NULL;
	returnTk.lineNo = 999;

	/* holder for ascii value of currently read character */
	int currentChar;

	/* DFAColumn where currentChar is represented in DFA table */
	int DFAColumn;
	/* string holder for multiple character tokens */
	/* +1 to allow over limit error reporting */
	char *currentWord = malloc(MAXIDLEN + 1);
	if(currentWord == NULL){
		printf("Unable To Allocate Memory For currentWord\n");
		exit(EXIT_FAILURE);
	}
	/* string to allow character to be appended easily to currentWord above */
	char *currentCharStr = malloc(LOOKAHEAD);
	if(currentCharStr == NULL){
		printf("Unable To Allocate Memory For currentCharStr\n");
		exit(EXIT_FAILURE);
	}

	/*
	Continnue to get characters from the file and build token
	while not a final state and not an error state
	*/
	while (currentState < 500 && currentState > -1) {
		currentChar = fgetc(infile);

		/*
		Handle comments first to prevent unnecessary work
		Comments are skipped beginning with # until space encountered
		*/
		if (currentChar == '#') {
			while (!isspace(currentChar)) {
				currentChar = fgetc(infile);
			}
		}

		/*
		If not a comment, look at currentChar to
		find applicable DFA table column
		*/
		DFAColumn = matchValid(currentChar);
  		sprintf(currentCharStr, "%c", currentChar);
		/* Set nextState */
		nextState = DFAStateTable[currentState][DFAColumn];

		/* Handle invalid characters at the start of a token */
		if (DFAColumn == -2) {
			returnTk.tkIdentifier = ERROR_tk;
			returnTk.lineNo = *lineNo;
			strcpy(currentWord, currentCharStr);
			printf("Scanner Error: Invalid Character: %c.\n", currentChar);
			returnTk.tkString = currentWord;
			exit(EXIT_FAILURE);
		}
		/* If next state is a final, EOF or an error */
		if (nextState > 499 || nextState == -1 || nextState == -2) {
			/* EOF */
			if (nextState == -1) {
				returnTk.tkIdentifier = EOF_tk;
				returnTk.lineNo = *lineNo;
				returnTk.tkString = "EOF";
				return returnTk;
			}
			/* Check for special case tokens */
			/* == , =!, <<, >> */
			/* 
			if special case, build the word, look up the token and
			write lineNo to result to overwrite default/testing value
			*/
			else if (nextState == 503 || nextState == 504 || nextState == 505 || nextState == 506) {
  				sprintf(currentCharStr, "%c", currentChar);
				strcat(currentWord, currentCharStr);
				returnTk = lookup(nextState, currentWord);
				returnTk.lineNo = *lineNo;
				return returnTk;
			}
			/* End special case tokens */
			/* "Normal" final States */
			else if (nextState > 499) {
				returnTk = lookup(nextState, currentWord);
				returnTk.lineNo = *lineNo;
				ungetc(currentChar, infile);
				return returnTk;
			}
			/* Catch Error (-2) */
			else {
				ungetc(currentChar, infile);
				int temp = fgetc(infile);
  				sprintf(currentCharStr, "%c", temp);
				strcat(currentWord, currentCharStr);
				printf("Scanner Error: Line: %i. Invalid Character: %c. %s Not A Valid Language Construct.\n", *lineNo, currentChar, currentWord);
				returnTk.tkIdentifier = ERROR_tk;
				returnTk.lineNo = *lineNo;
				returnTk.tkString = currentWord;
				exit(EXIT_FAILURE);
			}
		}
		/* Final state not yet reached. continue building token */
		else{
			if(!isspace(currentChar)){
			strcat(currentWord, currentCharStr);
			}
			/* Check to impose 8 character/digit length rule */
			if (strlen(currentWord) > MAXIDLEN){
				printf("Scanner Error: Identifier/Number Length Exception %s > 8\n", currentWord);
				returnTk.tkIdentifier = ERROR_tk;
				returnTk.lineNo = *lineNo;
				returnTk.tkString = "Length";
				exit(EXIT_FAILURE);
			}

			/* Handle \n */
			if (currentChar == '\n') {
				(*lineNo)++;
			}
			/* State transition */
			currentState = nextState;
		}
		
	}
	
	/* Gross error failure */
	returnTk.tkIdentifier = ERROR_tk;
	returnTk.lineNo = *lineNo;
	returnTk.tkString = "Scanner Failure";
	exit(EXIT_FAILURE);
}
/* End Scanner Function */
/*************************************************************************************************/
