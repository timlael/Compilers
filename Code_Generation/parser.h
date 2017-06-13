/*
    Tim M. Lael
    CS4280
    p4
    25-APR 2017
*/
/*
parser.h
header file file containing parser function prototypes
*/
/* Begin inclusion-prevention mechanism */
#ifndef PARSER_H
#define PARSER_H
#include "node.h"

/* Global input file pointer */
extern FILE *inputfp;

/* parser auxilary function prototype */
node_t *parser();

/* CFG function prototypes */
node_t *program();
node_t *block(int depth);
node_t *vars(int depth);
node_t *mvars(int depth);
node_t *expr(int depth);
node_t *m(int depth);
node_t *t(int depth);
node_t *f(int depth);
node_t *r(int depth);
node_t *stats(int depth);
node_t *mStat(int depth);
node_t *stat(int depth);
node_t *in(int depth);
node_t *out(int depth);
node_t *ifStatement(int depth);
node_t *loop(int depth);
node_t *assign(int depth);
node_t *relational(int depth);

/* node creation function prototype */
node_t *createNode(char *name, int depth);

/* End inclusion-prevention mechanism */
#endif /* PARSER_H */
