/*
    Tim M. Lael
    CS4280
    p2
    29-MAR 2017
*/
/*
utils.c
This is the source file containing function the definition for the concat
utility used throughout the program.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

/* Function to concatenate strings */
char *concat(const char *s1, const char *s2){
  char *ns = malloc(snprintf(NULL, 0, "%s %s", s1, s2) + 1);
  if(ns == NULL){
	printf("Unable To Allocate Memory For Concatenated String\n");
	exit(EXIT_FAILURE);
  }
  sprintf(ns, "%s %s", s1, s2);
  return ns;
}
