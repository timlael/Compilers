/*
    Tim M. Lael
    CS4280
    p1
    26-FEB 2017
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

#include "testScanner.h"

int main(int argc, char* argv[]){
  /*************************************************************************************************/
  /*  Data Setup Area                                                                              */
  /*************************************************************************************************/

  /* input and temp file pointers */
  FILE *inputfp = NULL; //global
  static const char *EXTENSION = ".4280E01";

  /* Character array to read lines into. Arbitrarily chosen max length of 255 */
  // char line[255];
  /* temporary file for storing read strings */
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
    printf("testScanner\t\t - Invokes program and user is prompted for input.\n");
    printf("\t\t\t Keyboard simulated EOF terminates input.\n\n");
    printf("testScanner < filename\t - Invokes program and input is read via stdin file redirection.\n");
    printf("\t\t\t filename must be precise including extension (if any).\n\n");
    printf("testScanner filename\t - Invokes program and input is read directly from file.\n");
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
	if(!feof(inputfp)){
		testScanner(inputfp);
	}
	fclose(inputfp);
}

  /* case else: file name (less extension) provided for input read */
  else{
    /* input file pointer points to file provided in argv[1] with implicit extension concated */
    char *fileName = malloc(sizeof(argv[1]) + sizeof(EXTENSION));
    strncat(fileName, argv[1], strlen(argv[1]));
    strncat(fileName, EXTENSION, strlen(EXTENSION));
    inputfp = fopen(fileName, "r");

    /* covers input file open failure */
    if (inputfp == NULL){
      fprintf(stderr, "Unable To Open '%s': %s\n", argv[1], strerror(errno));
      exit(EXIT_FAILURE);
    }

    /*
    start reading token at something other than white-space,
    and stop reading a single token when you get to whitespace
    */
	if(!feof(inputfp)){
		testScanner(inputfp);
    }

    if (feof(inputfp)){
      printf("\nFile Read Complete.\n\n");
      fclose(inputfp);
    }
  }
  /* End Command Line Argument Parsing AND Input Keyboard/File Reading */
  return 0;
}
