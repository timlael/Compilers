/*
    Tim M. Lael
    CS4280
    p3
    14-APR 2017
*/
/*
semantics.h
This is the source file containing function the definition for
stack and semantic operations
*/

/* Begin inclusion-prevention mechanism */
#ifndef SEMANTICS_H
#define SEMANTICS_H
#define MAXSTACKSIZE 100		/* Stack size limit per project spec */
#include "token.h"
#include "node.h"

/* Stack operations */
void push(token_t stack_tk);
void pop(int scopeStart);
void stackInit(void);
/* Variable/stack search operations */
int find(char *var);
int varExists(char *var);
/* Main semantic checking function */
void semanticCheck(node_t *node, int count);

/* End inclusion-prevention mechanism */
#endif /* SEMANTICS_H */
