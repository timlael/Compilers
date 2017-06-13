/*
    Tim M. Lael
    CS4280
    p2
    13-MAR 2017
*/
/*
main.c
Source file containing main(). Program master.
*/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "node.h"
#include "treePrint.h"

int main(int argc, char* argv[]){
	/*************************************************************************************************/
  	/*  Data Setup Area                                                                              */
  	/*************************************************************************************************/

	/* implicit extension */
  	static const char *EXTENSION = ".4280E01";
	/* Parse tree */
	node_t *tree;

  	/* End data setup */

  	/*************************************************************************************************/
  	/*  Command Line Argument Parsing AND Input Keyboard/File Reading                                */
  	/*************************************************************************************************/

  	/* Error on improper usage */
  	if (argc > 2){
  		printf("\n****************************************************************************************\n");
    	printf("                               Invalid number of arguments.\n");
    	printf("****************************************************************************************\n");
    	printf("Usage:\n");
    	printf("testFrontEnd\t\t - Invokes program and user is prompted for input.\n");
    	printf("\t\t\t Keyboard simulated EOF terminates input.\n\n");
    	printf("testFrontEnd < filename\t - Invokes program and input is read via stdin file redirection.\n");
    	printf("\t\t\t filename must be precise including extension (if any).\n\n");
    	printf("testFrontEnd filename\t - Invokes program and input is read directly from file.\n");
    	printf("\t\t\t filename should not include implicit extension (.4280E01)\n\n");
    	printf("****************************************************************************************\n");
    	exit(EXIT_FAILURE);
	}

  	/* case argc == 1: default run with no input file provided or redirection */
  	/* redirection requires explicit file naming or bash redirection will fail */
  	else if(argc == 1){
    	/* input file pointer points to stdin/redirect */
    	inputfp = stdin;


    	/* unlikely, but covers stdin open failure */
    	if (inputfp == NULL){
      		fprintf(stderr, "Unable To Open stdin: %s\n", strerror(errno));
      		exit(EXIT_FAILURE);
    	}

    	/* Ask user for input if not redirected */
    	if (isatty(0)){
      		printf("\nNo Filename Given. \nEnter Input: ");
    	}
		/* Call parser to build tree */
		if(!feof(inputfp)){
			tree = parser();
		}
		/* Print tree */
		preOrderPrint(tree);
		fclose(inputfp);
	}

  	/* case else: file name (less extension) provided for input read */
  	else{
    	/* input file pointer points to file provided in argv[1] with implicit extension concated */
    	char *fileName = malloc(sizeof(argv[1]) + sizeof(EXTENSION));
		if(fileName == NULL){
			printf("Unable To Allocate Memory For Input File Pointer\n");
			exit(EXIT_FAILURE);
		}
    	strncat(fileName, argv[1], strlen(argv[1]));
    	strncat(fileName, EXTENSION, strlen(EXTENSION));
    	inputfp = fopen(fileName, "r");

    	/* covers input file open failure */
    	if (inputfp == NULL){
      		fprintf(stderr, "Unable To Open '%s%s': %s\n", argv[1], EXTENSION, strerror(errno));
      		exit(EXIT_FAILURE);
    	}

    	/* Call Parser and build tree */
		if(!feof(inputfp)){
			tree = parser();
		}
		/* Print Tree */
		preOrderPrint(tree);
		fclose(inputfp);
  	}
  	/* End Command Line Argument Parsing AND Input Keyboard/File Reading */
  	return 0;
}
