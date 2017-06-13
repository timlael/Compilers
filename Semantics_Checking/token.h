/*
    Tim M. Lael
    CS4280
    p3
    14-APR 2017
*/
/*
token.h
header file file containing alphabet/character/token/keyword definitions
*/

/* Begin inclusion-prevention mechanism */
#ifndef TOKEN_H
#define TOKEN_H

/* Enumeration of tokens */ 
typedef enum tokenId {
	/* EOF, Error */
	EOF_tk, ERROR_tk,
	/* Identifiers and Numbers */
	IDENT_tk, NUM_tk,
    /* Operators */
    EQUAL_tk, DBLEQUAL_tk, EQUALBANG_tk, DBLLARROW_tk, DBLRARROW_tk, 
    COLON_tk, PLUS_tk, MINUS_tk, ASTERISK_tk, SLASH_tk, 
    AMP_tk, PCT_tk, PERIOD_tk,
    /* Delimiters */
    LPAREN_tk, RPAREN_tk, COMMA_tk, LBRACE_tk, RBRACE_tk, 
	SCOLON_tk, LBRACKET_tk, RBRACKET_tk,
	/* Keywords */
	START_tk, STOP_tk, DECISION_tk, WHILE_tk, VOID_tk, INT_tk,
	RETURN_tk, READ_tk, PRINT_tk, PROGRAM_tk
} tok_id;

/* Token definition */
typedef struct token{
	tok_id tkIdentifier; 
	char *tkString;
	int lineNo;
} token_t;
						
/* End inclusion-prevention mechanism */
#endif /* TOKEN_H */
