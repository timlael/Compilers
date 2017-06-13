/*
    Tim M. Lael
    CS4280
    p1
    26-FEB 2017
*/
/*
testScanner.c
source file containing scanner testing functions
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testScanner.h"
#include "token.h"
#include "scanner.h"

/* 
Token name array to display on terminal feedback
Mapped to token enumeration order
*/
char *tokenNames[35] = {
"End of File", "Error", "Identifier", "Number", "Equal Sign",
"Double Equal Sign", "Equal Bang", "Double Left Arrow", "Double Right Arrow", "Colon",
"Plus Sign", "Minus Sign", "Asterisk", "Slash", "Ampersand",
"Percent Sign", "Period", "Left Parenthesis", "Right Parenthesis", "Comma",
"Left Brace", "Right Brace", "Semicolon", "Left Bracket", "Right Bracket",
"start Keyword", "stop Keyword", "decision Keyword", "while Keyword", "void Keyword",
"int Keyword", "return keyword", "read Keyword", "print Keyword", "program Keyword"
};

/* testScanner function calls scanner while input file !eof iand reports returned token values*/
void testScanner (FILE *infile){
	token_t temp;
	int lineNo = 1;

	do{
		temp = scan(infile, &lineNo);
		if(strcmp(tokenNames[temp.tkIdentifier], "Error") == 0){
		printf("Scanner Error: %s Not Allowed At Line: %i.\n", temp.tkString, temp.lineNo);
			printf("Exiting\n");
			exit(EXIT_FAILURE);
		}
		else{
		printf("Read %s Token With Value: %s At Line %i.\n", tokenNames[temp.tkIdentifier], temp.tkString, temp.lineNo);
		}
	} while(!feof(infile));
}
