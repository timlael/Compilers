/*
    Tim M. Lael
    CS4280
    p3
    14-APR 2017
*/
/*
treePrint.c
source file containing tree printing function
*/
#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "utils.h"

void preOrderPrint(node_t *root){
	/* If tree is empty, return */
	if (root == NULL){
		printf("Print Error: You Have Attempted To Print An Empty Tree\n");
		return;
	}
	/* If node has both primary and secondary tokens, place them behind the node name for printing */
	if(root->token.tkString != NULL && root->token2.tkString != NULL){
		printf("%*s%-9s\n", root->depth*2," ", concat(concat(root->name, root->token.tkString), root->token2.tkString));
	}
	/* If only one token exists, place it behind the node name for printing */
	else if(root->token.tkString != NULL && root->token2.tkString == NULL){
		printf("%*s%-9s\n", root->depth*2," ", concat(root->name, root->token.tkString));
	}
	/* If there are no associated tokens, print the node name */
	else{
		printf("%*s%-9s\n", root->depth*2," ", root->name);
	}
	/* Print children in order from L to R */
	if (root->child1 != NULL)
		preOrderPrint(root->child1);
	if (root->child2 != NULL)
		preOrderPrint(root->child2);
	if (root->child3 != NULL)
		preOrderPrint(root->child3);
	if (root->child4 != NULL)
		preOrderPrint(root->child4);
}

