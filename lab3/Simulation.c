/*
 *  File: Simulation.c
 *  Last update : 19 February 2020 Nat Choeypant
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* If there are custom classes/source files that you write, with
   custom functions, and you want those functions available for use in
   THIS .c file, then include the header file for the custom .c
   file(s) you've written, using the #include directive. For example:

   #include "SomeFile.h"

 */

int demotionCounter;
int dispatchRatio;

void Simulate(char *filename,int demotionCounter, int dispatchRatio) {
  // A function whose input is the demotion counter for a queue A and the
  // dispatch ratio between the two queueus
  printf("Input file name : %s\n", filename);
  printf("Demotion counter value received : %d\n", demotionCounter);
  printf("Dispatch ratio value received : %d\n", dispatchRatio); 
}

int main(int argc, char*argv[]) {
  if(argc != 4){
    puts("Incorrect number of input arguments. Exit Main");
    return(0);
  }
  char *filename = argv[1];
  demotionCounter = atoi(argv[2]);
  dispatchRatio = atoi(argv[3]);
  Simulate(filename,demotionCounter,dispatchRatio);
  exit(0);
}
