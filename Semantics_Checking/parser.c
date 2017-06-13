/*
    Tim M. Lael
    CS4280
    p3
    14-APR 2017
*/
/*
parser.c
source file containing parsing functions
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "scanner.h"
#include "token.h"

/* define input file pointer */
FILE *inputfp = NULL;
/* declare token_t and lineNo to be used in scanner calls within CFG functions */
token_t tk;
int lineNo = 1;

/* Array of token literals mapped to token enums for error reprting purposes */
static const char *tokenNames[] ={
	[EOF_tk] = "EOF_tk", [ERROR_tk] = "ERROR_tk",
	[IDENT_tk] = "IDENT_tk", [NUM_tk] = "NUM_tk",
    [EQUAL_tk] = "EQUAL_tk", [DBLEQUAL_tk] = "DBLEQUAL_tk", [EQUALBANG_tk] = "EQUALBANG_tk", 
	[DBLLARROW_tk] = "DBLLARROW_tk", [DBLRARROW_tk] = "DBLRARROW_tk", 
    [COLON_tk] = "COLON_tk", [PLUS_tk] = "PLUS_tk", [MINUS_tk] = "MINUS_tk", 
	[ASTERISK_tk] = "ASTERISK_tk", [SLASH_tk] = "SLASH_tk", 
    [AMP_tk] = "AMP_tk", [PCT_tk] = "PCT_tk", [PERIOD_tk] = "PERIOD_tk",
    [LPAREN_tk] = "LPAREN_tk", [RPAREN_tk] = "RPAREN_tk", [COMMA_tk] = "COMMA_tk", 
	[LBRACE_tk] = "LBRACE_tk", [RBRACE_tk] = "RBRACE_tk", 
	[SCOLON_tk] = "SCOLON_tk", [LBRACKET_tk] = "LBRACKET_tk", [RBRACKET_tk] = "RBRACKET_tk",
	[START_tk] = "START_tk", [STOP_tk] = "STOP_tk", [DECISION_tk] = "DECISION_tk", 
	[WHILE_tk] = "WHILE_tk", [VOID_tk] = "VOID_tk", [INT_tk] = "INT_tk",
	[RETURN_tk] = "RETURN_tk", [READ_tk] = "READ_tk", [PRINT_tk] = "PRINT_tk", [PROGRAM_tk] = "PROGRAM_tk"
};

/* Main parser call */
node_t *parser(){
	/* Root node to be returned as tree */
	node_t *node;
	/* request token from scanner (all sub-functions expect fresh token) */
	tk = scan(inputfp, &lineNo);
	/* call first CFG function and assign its return to root node */
	node = program();
	/* EOF signals successful parse */
	if(tk.tkIdentifier != EOF_tk){
	/* Anything returned other than EOF means parse failed */
		printf("Parser Error: Parse Failed. Expected EOF_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
		exit(EXIT_FAILURE);
	}		
	return node;
}

/* program() looks for program keyword to signal start of valid program */
node_t *program(){
	/* Tree depth is 0 here */
	int depth = 0;
	/* Create tree node for this call */
	node_t *node = createNode("<program>", depth);
	/* Look for expected program token */
	if(tk.tkIdentifier == PROGRAM_tk){										/* Predict <program> -> program <vars> <block> */
		/* Get fresh token (consume PROGRAM_tk) for next function */
		tk = scan(inputfp, &lineNo);
		/* call child functions and populate children with returned nodes */
		node->child1 = vars(depth);
		node->child2 = block(depth);
		return node;
	}
	/* If token other than expected encountered, raise error with line number and exit */
	else{
		printf("Parser Error: Invalid Token, Expected PROGRAM_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
		exit(EXIT_FAILURE);
	}
}

/* 
All other functions are similar to program(). I will comment only on functionality
that is unique to the function where it occurs
*/

node_t *vars(int depth){
	depth++;
	node_t *node = createNode("<vars>", depth);
	if(tk.tkIdentifier == INT_tk){											/* Predict <vars> -> int identifier <mvars> */
		tk = scan(inputfp, &lineNo);
		if(tk.tkIdentifier == IDENT_tk){
			/* Store non-syntactic token */
			node->token = tk;
			tk = scan(inputfp, &lineNo);
			node->child1 = mvars(depth);
			return node;
		}
		else{
			printf("Parser Error: Invalid Token, Expected IDENT_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
		}
	}
	return NULL;															/* Predict <vars> -> empty */
}

node_t *block(int depth){
	depth++;
	node_t *node = createNode("<block>", depth);
	if(tk.tkIdentifier == START_tk){										/* Predict <block> -> start <vars> <stats> stop */	
		tk = scan(inputfp, &lineNo);
		node->child1 = vars(depth);
		node->child2 = stats(depth);
		if (tk.tkIdentifier == STOP_tk){
			tk = scan(inputfp, &lineNo);
			return node;
		}
		else{
			printf("Parser Error: Invalid Token, Expected STOP_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Parser Error: Invalid Token, Expected START_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
		exit(EXIT_FAILURE);
	}
}

node_t *mvars(int depth){													
	depth++;
	node_t *node = createNode("<mvars>", depth);							
	if(tk.tkIdentifier == COLON_tk){										/* Predict <mvars> -> : identifier <mvars> */
		tk = scan(inputfp, &lineNo);
		if(tk.tkIdentifier == IDENT_tk){
			node->token = tk;
			tk = scan(inputfp, &lineNo);
			node->child1 = mvars(depth);
			return node;
		}
		else{
			printf("Parser Error: Invalid Token, Expected IDENT_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
			exit(EXIT_FAILURE);
		}
	}
	else{
		return NULL;														/* Predict <mvars> -> empty */
	}
}

node_t *expr(int depth){
	depth++;
	node_t *node = createNode("<expr>", depth);
	node->child1 = m(depth);												/* Predict <expr> -> <M> */
	if(tk.tkIdentifier == ASTERISK_tk){										/* Predict <expr> -> <M> * <expr> */
		node->token = tk;
		tk = scan(inputfp, &lineNo);
		node->child2 = expr(depth);
		return node;
	}
	else{
		return node;
	}
}

node_t *m(int depth){
	depth++;
	node_t *node = createNode("<M>", depth);
	node->child1 = t(depth);												/* Predict <M> -> <T> */
	if(tk.tkIdentifier == SLASH_tk){										/* Predict <M> -> <T> / <M> */
		node->token = tk;
		tk = scan(inputfp, &lineNo);
		node->child2 = m(depth);
		return node;
	}
	else{
		return node;
	}
}

node_t *t(int depth){
	depth++;
	node_t *node = createNode("<T>", depth);
	node->child1 = f(depth);												/* Predict <T> -> <F> */
	if(tk.tkIdentifier == PLUS_tk){											/* Predict <T> -> <F> + <T> */
		node->token = tk;
		tk = scan(inputfp, &lineNo);
		node->child2 = t(depth);
		return node;
	}
	else if (tk.tkIdentifier == MINUS_tk){									/* Predict <T> -> <F> - <T> */
		node->token = tk;
		tk = scan(inputfp, &lineNo);
		node->child2 = t(depth);
		return node;
	}
	else{
		return node;
	}
}

node_t *f(int depth){
	depth++;
	node_t *node = createNode("<F>", depth);
	if(tk.tkIdentifier == AMP_tk){											/* Predict <F> -> & <F> */
		node->token = tk;
		tk = scan(inputfp, &lineNo);
		node->child1 = f(depth);
		return node;
	}
	else{
		node->child1 = r(depth);											/* Predict <F> -> <R> */
		return node;
	}
}

node_t *r(int depth){
	depth++;
	node_t *node = createNode("<R>", depth);
	if(tk.tkIdentifier == LPAREN_tk){										/* Predict <R> -> (<expr>) */
		tk = scan(inputfp, &lineNo);
		node->child1 = expr(depth);
		if(tk.tkIdentifier == RPAREN_tk){
			tk = scan(inputfp, &lineNo);
			return node;
		}
		else{
			printf("Parser Error: Invalid Token, Expected RPAREN_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
			exit(EXIT_FAILURE);
		}
	}
	else if(tk.tkIdentifier == IDENT_tk){									/* Predict <R> -> Identifier */
		node->token = tk;
		tk = scan(inputfp, &lineNo);
		return node;
	}
	else if(tk.tkIdentifier == NUM_tk){										/* Predict <R> -> Number */
		node->token = tk;
		tk = scan(inputfp, &lineNo);
		return node;
	}
	else{
		printf("Parser Error: Invalid Token, Expected LPAREN_tk, IDENT_tk or NUM_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
		exit(EXIT_FAILURE);
	}
}

node_t *stats(int depth){													/* k=0 No prediction */
	depth++;																/* <stats> -> <stat> <mStat> */
	node_t *node = createNode("<stats>", depth);
	node->child1 = stat(depth);
	node->child2 = mStat(depth);
	return node;
}

node_t *mStat(int depth){
	depth++;
	node_t *node = createNode("<mStat>", depth);
	/*
	Predict <mStat> -> <stat> <mStat>
	process mStat follow to determine empty prediction 
	*/
	if(tk.tkIdentifier == READ_tk || tk.tkIdentifier == PRINT_tk || tk.tkIdentifier == START_tk ||
	tk.tkIdentifier == DECISION_tk || tk.tkIdentifier == WHILE_tk || tk.tkIdentifier == IDENT_tk){
		node->child1 = stat(depth);
		node->child2 = mStat(depth);
		return node;
	}
	else{
		return NULL;														/* Predict <mStat> -> empty */	
	}
}

node_t *stat(int depth){
	depth++;
	node_t *node = createNode("<stat>", depth);
	if(tk.tkIdentifier == READ_tk){											/* Predict <stat> -> <in> */
		node->child1 = in(depth);
		return node;
	}
	else if(tk.tkIdentifier == PRINT_tk){									/* Predict <stat> -> <out> */
		node->child1 = out(depth);
		return node;
	}
	else if(tk.tkIdentifier == START_tk){									/* Predict <stat> -> <block> */
		node->child1 = block(depth);
		return node;
	}
	else if(tk.tkIdentifier == DECISION_tk){								/* Predict <stat> -> <if> */
		node->child1 = ifStatement(depth);
		return node;
	}
	else if(tk.tkIdentifier == WHILE_tk){									/* Predict <stat> -> <loop> */
		node->child1 = loop(depth);
		return node;
	}
	else if(tk.tkIdentifier == IDENT_tk){									/* Predict <stat> -> <assign> */
		node->child1 = assign(depth);
		return node;
	}
	else{
		printf("Parser Error: Invalid Token, Expected READ_tk, PRINT_tk, START_tk, DECISION_tk, WHILE_tk or IDENT_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
		exit(EXIT_FAILURE);
	}
}

node_t *in(int depth){														/* k=0 No prediction */
	depth++;																/* <in> -> read >> Identifier ; */
	node_t *node = createNode("<in>", depth);
	if(tk.tkIdentifier == READ_tk){
		tk = scan(inputfp, &lineNo);
		if(tk.tkIdentifier == DBLRARROW_tk){
			tk = scan(inputfp, &lineNo);
			if(tk.tkIdentifier == IDENT_tk){
				node->token = tk;
				tk = scan(inputfp, &lineNo);
				if(tk.tkIdentifier == SCOLON_tk){
					tk = scan(inputfp, &lineNo);
					return node;
				}
				else{
					printf("Parser Error: Invalid Token, Expected SCOLON_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
					exit(EXIT_FAILURE);
				}
			}	
			else{
				printf("Parser Error: Invalid Token, Expected IDENT_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
				exit(EXIT_FAILURE);
			}
		}
		else{
			printf("Parser Error: Invalid Token, Expected DBLRARROW_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Parser Error: Invalid Token, Expected READ_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
		exit(EXIT_FAILURE);
	}
}

node_t *out(int depth){														/* k=0 No prediction */
	depth++;																/* <out> -> print << <expr> */
	node_t *node = createNode("<out>", depth);
	if(tk.tkIdentifier == PRINT_tk){
		tk = scan(inputfp, &lineNo);
		if(tk.tkIdentifier == DBLLARROW_tk){
			tk = scan(inputfp, &lineNo);
			node->child1 = expr(depth);
			return node;
		}
		else{
			printf("Parser Error: Invalid Token, Expected DBLLARROW_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Parser Error: Invalid Token, Expected PRINT_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
		exit(EXIT_FAILURE);
	}
}

node_t *ifStatement(int depth){												/* k=0 No prediction */
	depth++;																/* <if> -> decision ( <expr> <RO> <expr> ) <block> */
	node_t *node = createNode("<if>", depth);
	if(tk.tkIdentifier == DECISION_tk){
		tk = scan(inputfp, &lineNo);
		if(tk.tkIdentifier == LPAREN_tk){
			tk = scan(inputfp, &lineNo);
			node->child1 = expr(depth);
			node->child2 = relational(depth);
			node->child3 = expr(depth);
			if(tk.tkIdentifier == RPAREN_tk){
				tk = scan(inputfp, &lineNo);
				node->child4 = block(depth);
				return node;
			}
			else{
				printf("Parser Error: Invalid Token, Expected RPAREN_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
				exit(EXIT_FAILURE);
			}
		}
		else{
			printf("Parser Error: Invalid Token, Expected LPAREN_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Parser Error: Invalid Token, Expected DECISION_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
		exit(EXIT_FAILURE);
	}
}

node_t *loop(int depth){													/* k=0 No prediction */	
	depth++;																/* <loop> -> while ( <expr> <RO> <expr> ) <block> */
	node_t *node = createNode("<loop>", depth);
	if(tk.tkIdentifier == WHILE_tk){
		tk = scan(inputfp, &lineNo);
		if(tk.tkIdentifier == LPAREN_tk){
			tk = scan(inputfp, &lineNo);
			node->child1 = expr(depth);
			node->child2 = relational(depth);
			node->child3 = expr(depth);
			if(tk.tkIdentifier == RPAREN_tk){
				tk = scan(inputfp, &lineNo);
				node->child4 = block(depth);
				return node;
			}
			else{
				printf("Parser Error: Invalid Token, Expected RPAREN_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
				exit(EXIT_FAILURE);
			}
		}
		else{
			printf("Parser Error: Invalid Token, Expected LPAREN_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
			exit(EXIT_FAILURE);
		}

	}
	else{
		printf("Parser Error: Invalid Token, Expected WHILE_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
		exit(EXIT_FAILURE);
	}
}

node_t *assign(int depth){													/* k=0 No prediction */
	depth++;																/* <assign> -> Identifier = <expr> ; */
	node_t *node = createNode("<assign>", depth);
	if(tk.tkIdentifier == IDENT_tk){
		node->token = tk;
		tk = scan(inputfp, &lineNo);
		if(tk.tkIdentifier == EQUAL_tk){
			tk = scan(inputfp, &lineNo);
			node->child1 = expr(depth);
			if(tk.tkIdentifier == SCOLON_tk){
				tk = scan(inputfp, &lineNo);
				return node;
			}
			else{
				printf("Parser Error: Invalid Token, Expected SCOLON_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
				exit(EXIT_FAILURE);
			}
		}
		else{
			printf("Parser Error: Invalid Token, Expected EQUAL_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Parser Error: Invalid Token, Expected IDENT_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
		exit(EXIT_FAILURE);
	}
}

node_t *relational(int depth){
	depth++;
	node_t *node = createNode("<RO>", depth);
	if(tk.tkIdentifier == DBLLARROW_tk){									/* Predict <RO> -> << */
		node->token = tk;
		tk = scan(inputfp, &lineNo);
		if(tk.tkIdentifier == EQUAL_tk){									/* Predict <RO> -> <<= */
			/* Store secondary non-syntactic token */
			node->token2 = tk;
			tk = scan(inputfp, &lineNo);
			return node;
		}
		return node;
	}
	else if(tk.tkIdentifier == DBLRARROW_tk){								/* Predict <RO> -> >> */
		node->token = tk;
		tk = scan(inputfp, &lineNo);
		if(tk.tkIdentifier == EQUAL_tk){									/* Predict <RO> -> >>= */
			node->token2 = tk;
			tk = scan(inputfp, &lineNo);
			return node;
		}
		return node;
	}
	else if(tk.tkIdentifier == DBLEQUAL_tk){								/* Predict <RO> -> == */
		node->token = tk;
		tk = scan(inputfp, &lineNo);
		return node;
	}
	else if(tk.tkIdentifier == EQUALBANG_tk){								/* Predict <RO> -> =! */
		node->token = tk;
		tk = scan(inputfp, &lineNo);
		return node;
	}
	else{
		printf("Parser Error: Invalid Token, Expected DBLLARROW_tk, DBLRARROW_tk, DBLEQUAL_tk or EQUALBANG_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
		exit(EXIT_FAILURE);
	}
}

/* New node creation function */
node_t *createNode(char *name, int depth){
	node_t *newNode;
	newNode = malloc(sizeof(node_t));
	if(newNode == NULL){
		printf("Node Allocation Error. Out Of Memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(newNode->name, name);
	newNode->depth = depth;
	newNode->child1 = NULL;
	newNode->child2 = NULL;
	newNode->child3 = NULL;
	newNode->child4 = NULL;
	return newNode;
}
