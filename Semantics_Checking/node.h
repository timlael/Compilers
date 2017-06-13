/*
    Tim M. Lael
    CS4280
    p3
    14-APR 2017
*/
/*
node.h
header file file containing node definition
*/
/* Begin inclusion-prevention mechanism */
#ifndef NODE_H
#define NODE_H
#include "token.h"
#define NAMELENGTH 10 					/* Maximum node name length (added 2 to account for <> tags */ 

typedef struct node{
	/* node name and depth */
	char name[NAMELENGTH];
	int depth;
	/* token storage for non-syntactic tokens */
	token_t token;
	token_t token2;
	/* <if> and <loop> can have 4 children */
	struct node *child1;
	struct node *child2;
	struct node *child3;
	struct node *child4;
} node_t;

/* End inclusion-prevention mechanism */
#endif /* NODE_H */
