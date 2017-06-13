/*
    Tim M. Lael
    CS4280
    p4
    25-APR 2017
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
#include "semantics.h"

/* extern .asm file pointer */
FILE *outfile;

int main(int argc, char* argv[]){
	/*************************************************************************************************/
  	/*  Data Setup Area                                                                              */
  	/*************************************************************************************************/

	/* implicit extension */
  	static const char *EXTENSION = ".4280E01";
	/* Parse tree */
	node_t *tree;
	/* Stack Initialization */
	stackInit();
	/* Temp var array Initialization */
	tempArrInit();

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
    	printf("comp\t\t\t - Invokes program and user is prompted for input.\n");
    	printf("\t\t\t Keyboard simulated EOF terminates input.\n\n");
    	printf("comp < filename\t - Invokes program and input is read via stdin file redirection.\n");
    	printf("\t\t\t filename must be precise including extension (if any).\n\n");
    	printf("comp filename\t - Invokes program and input is read directly from file.\n");
    	printf("\t\t\t filename should not include implicit extension (.4280E01)\n\n");
    	printf("****************************************************************************************\n");
    	exit(EXIT_FAILURE);
	}

  	/* case argc == 1: default run with no input file provided or redirection */
  	/* redirection requires explicit file naming or bash redirection will fail */
  	else if(argc == 1){
    	/* input file pointer points to stdin/redirect */
    	inputfp = stdin;
		/* no input/redirection output file creation */
		char *outName = "out.asm";
		outfile = fopen(outName, "w");

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
		/* Check Semantics */
		semanticCheck(tree, 0);
      	printf("Semantic Check Complete. Code Generated in %s\n", outName);

		fclose(inputfp);
		fclose(outfile);
	}

  	/* case else: file name (less extension) provided for input read */
  	else{
    	/* input file pointer points to file provided in argv[1] with implicit extension concated */
    	char *fileName = malloc(strlen(argv[1]) + strlen(EXTENSION) + 2);
    	char *outName = malloc(strlen(argv[1]) + strlen(".asm") + 2);
		if(fileName == NULL){
			printf("Unable To Allocate Memory For Input File Pointer\n");
			exit(EXIT_FAILURE);
		}
		/* implicit named input file handling */
    	strncat(fileName, argv[1], strlen(argv[1]));
    	strncat(fileName, EXTENSION, strlen(EXTENSION));
    	inputfp = fopen(fileName, "r");
		/* output file creation for implicitly named input file */
    	strncat(outName, argv[1], strlen(argv[1]));
    	strncat(outName, ".asm", strlen(".asm"));
		outfile = fopen(outName, "w");

    	/* covers input file open failure */
    	if (inputfp == NULL){
      		fprintf(stderr, "Unable To Open '%s%s': %s\n", argv[1], EXTENSION, strerror(errno));
      		exit(EXIT_FAILURE);
    	}

    	/* Call Parser and build tree */
		if(!feof(inputfp)){
			tree = parser();
		}
		/* Check Semantics and Generate Code */
		semanticCheck(tree, 0);
      	printf("Semantic Check Complete. Code Generated in %s\n", outName);

		fclose(inputfp);
		fclose(outfile);
  	}
  	/* End Command Line Argument Parsing AND Input Keyboard/File Reading */
  	return 0;
}
