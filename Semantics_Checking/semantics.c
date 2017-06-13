/*
    Tim M. Lael
    CS4280
    p3
    14-APR 2017
*/
/*
semantics.c
This is the source file containing function the definition for
stack and semantic operations
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantics.h"

/* Initialize array of tokens to act as stack */
token_t stack[MAXSTACKSIZE];
/* Total number of vars on the stack. Current stack size */
static int stackVars = 0;
/* Used to track/assign scope starting point on stack */
static int scopeStart = 0;
/* Loop counter var */
int i = 0;

/* Initialize all stack location strings to prevent comparison to uninitialized memory location(seg fault) */
void stackInit(void){
	for(i = 0; i < MAXSTACKSIZE + 1; i++){
		stack[i].tkString = "";
	}
	return;
}

/* Stack push */
void push(token_t stack_tk){
	/* Warn stack overflow */
	if(stackVars == MAXSTACKSIZE){
		printf("Stack Is Full\n");
		exit(EXIT_FAILURE);
	}
	/* Check for variable scope existence before insertion */
	for(i = scopeStart; i < stackVars; i++){
		if(strcmp(stack[i].tkString, stack_tk.tkString) == 0){
			printf("%s Already Declared In This Scope On Line: %d\n", stack_tk.tkString, stack[i].lineNo);
			exit(EXIT_FAILURE);
		}
	}	
	/* Push token to the stack */
	stack[stackVars] = stack_tk;
	/* Increment number of variables on stack to keep stack size current */
	stackVars++;
}

/* Pop scope vars when called */
void pop(int scopeStart){
	for(i = stackVars; i > scopeStart; i--){
		/* Decrement number of variables on stack to simulate removal and keep stack size current */
		stackVars--;
		stack[i].tkString = NULL;
	}
}

/* Make sure variable doesn't exist in the local scope stack before pushing onto stack */
int find(char *var){
	/* Search current scope for var string and return position */
	for(i = stackVars; i > scopeStart - 1; i--){
		if(strcmp(stack[i].tkString, var) == 0){
			return stackVars - 1 - i;
		}
	}
	/* Not found */
	return -1;
}

/* Make sure variable exists in scope (or parent) before use */
int varExists(char *var){
	/* Search current scope for var string and return position */
	for(i = stackVars - 1; i > - 1; i--){
		if(strcmp(stack[i].tkString, var) == 0){
			return stackVars - 1 - i;
		}
	}
	/* Not found */
	return -1;
}

void semanticCheck(node_t *node, int count){
	/* Empty Tree */
	if(node == NULL){
		return;
	}
	/* Process program node */
	if(strcmp(node->name, "<program>") == 0){
		/* Start of program has no vars */
		int numVars = 0;
		/* Process expected possible number of children if not null (<program> can have two child nodes) */
		if(node->child1 != NULL)
		semanticCheck(node->child1, numVars);
		if(node->child2 != NULL)
		semanticCheck(node->child2, numVars);
	}
	/* Process vars node */
	else if(strcmp(node->name, "<vars>") == 0){
		int found;
		scopeStart = stackVars;
		found = find(node->token.tkString);
		/* If not found in scope or defined globally, push onto stack */
		if(found == -1 || found > count){
			push(node->token);
			count += 1;
		}
		/* Error/exit if already defined within scope */
		else if(found < count){
			printf("'%s' Is Already Defined Within This Scope On Line: %d\n", node->token.tkString, node->token.lineNo);
			exit(EXIT_FAILURE);
		}
		/* Process expected possible number of children if not null (<vars> can have one child node) */
		if(node->child1 != NULL)
		semanticCheck(node->child1, count);
	}
	/* Process block node */
	else if(strcmp(node->name, "<block>") == 0){
		/* New block resets scope and number of variables in scope */
		int numVars = 0;
		/* Set scope = TOS */
		scopeStart = stackVars;
		/* Process expected possible number of children if not null (<block> can have two child nodes) */
		if(node->child1 != NULL)
		semanticCheck(node->child1, numVars);
		if(node->child2 != NULL)
		semanticCheck(node->child2, numVars);
		/* Pop scope variables before exit */
		pop(scopeStart);
	}	

/* 
All other functions are similar. I will comment only on functionality
that is unique to the function where it occurs
*/

	else if(strcmp(node->name, "<mvars>") == 0){
		int found;
		if(count > 0){
			found = find(node->token.tkString);
			if(found == -1 || found > count){
				push(node->token);
				count += 1;
			}
			else if(found < count){
				printf("'%s' Is Already Defined Within This Scope On Line: %d\n", node->token.tkString, node->token.lineNo);
				exit(EXIT_FAILURE);
			}
		}
		if(node->child1 != NULL)
		semanticCheck(node->child1, count);
	}
	else if(strcmp(node->name, "<expr>") == 0){
		/* Process as correct prediction using info from parse*/
		if(node->token.tkIdentifier == ASTERISK_tk){
			if(node->child1 != NULL)
			semanticCheck(node->child1, count);
			if(node->child2 != NULL)
			semanticCheck(node->child2, count);	
		}
		else{
			if(node->child1 != NULL)
			semanticCheck(node->child1, count);
		}
	}
	else if(strcmp(node->name, "<M>") == 0){
		if(node->token.tkIdentifier == SLASH_tk){
			if(node->child1 != NULL)
			semanticCheck(node->child1, count);
			if(node->child2 != NULL)
			semanticCheck(node->child2, count);	
		}
		else{
			if(node->child1 != NULL)
			semanticCheck(node->child1, count);
		}
	}	
	else if(strcmp(node->name, "<T>") == 0){
		if(node->token.tkIdentifier == PLUS_tk){
			if(node->child1 != NULL)
			semanticCheck(node->child1, count);
			if(node->child2 != NULL)
			semanticCheck(node->child2, count);	
		}
		if(node->token.tkIdentifier == MINUS_tk){
			if(node->child1 != NULL)
			semanticCheck(node->child1, count);
			if(node->child2 != NULL)
			semanticCheck(node->child2, count);	
		}
		else{
			if(node->child1 != NULL)
			semanticCheck(node->child1, count);
		}
	}	
	else if(strcmp(node->name, "<F>") == 0){
		if(node->token.tkIdentifier == AMP_tk){
			if(node->child1 != NULL)
			semanticCheck(node->child1, count);
		}
		else{
			if(node->child1 != NULL)
			semanticCheck(node->child1, count);
		}
	}
	else if(strcmp(node->name, "<R>") == 0){
		/* Check to make sure identifier was declared before use */
		if(node->token.tkIdentifier == IDENT_tk){
			int exists = varExists(node->token.tkString);
			if(exists == -1){
				printf("Variable '%s' On Line: %d Was Not Declared In This Scope.\n", node->token.tkString, node->token.lineNo);
				exit(EXIT_FAILURE);
			}
		}
		else{
			if(node->child1 != NULL)
			semanticCheck(node->child1, count);
		}
	}
	else if(strcmp(node->name, "<in>") == 0){
		int exists = varExists(node->token.tkString);
		if(exists == -1){
			printf("Variable '%s' On Line: %d Was Not Declared In This Scope.\n", node->token.tkString, node->token.lineNo);
			exit(EXIT_FAILURE);
		}	
	}
	else if(strcmp(node->name, "<out>") == 0){
		if(node->child1 != NULL)
		semanticCheck(node->child1, count);
	}
	else if(strcmp(node->name, "<if>") == 0){
		if(node->child1 != NULL)
		semanticCheck(node->child1, count);
		if(node->child3 != NULL)
		semanticCheck(node->child3, count);
		if(node->child4 != NULL)
		semanticCheck(node->child4, count);
	}	
	else if(strcmp(node->name, "<loop>") == 0){
		if(node->child1 != NULL)
		semanticCheck(node->child1, count);
		if(node->child3 != NULL)
		semanticCheck(node->child3, count);
		if(node->child4 != NULL)
		semanticCheck(node->child4, count);
	}	
	else if(strcmp(node->name, "<assign>") == 0){
		int exists = varExists(node->token.tkString);
		if(exists == -1){
			printf("Variable '%s' On Line: %d Was Not Declared In This Scope.\n", node->token.tkString, node->token.lineNo);
			exit(EXIT_FAILURE);
		}
		if(node->child1 != NULL)
		semanticCheck(node->child1, count);
	}
	/* Catch all to process productions with only non-terminals */
	else{
		if(node->child1 != NULL)
		semanticCheck(node->child1, count);
		if(node->child2 != NULL)
		semanticCheck(node->child2, count);
		if(node->child3 != NULL)
		semanticCheck(node->child3, count);
		if(node->child4 != NULL)
		semanticCheck(node->child4, count);
	}
	return;
}
