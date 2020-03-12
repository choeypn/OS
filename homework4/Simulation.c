#include <stdio.h>
#include <stdlib.h>

//#include "simulation.h"
  
//a structure for pagetable
typedef struct pageTable{
  int* timeStamp;
  char** pageNumber;
  int* ASID;
}pageTable;


//get input arg for allocation
//return allocation type from input arg
char* getAllocationMode(char in){
  char *alc;
  if(in == 's')
    alc = "Split";
  else if(in == 'f')
    alc = "Free";
  else
    alc = "Invalid input, go with free for all";
  return alc;
}
//function that processes input file line 
//print incoming request page number of file line
void processLine(pageTable* inv,FILE *f,int flag){
  char address[6];
  char *c = fgets(address,sizeof(address),f);
  if(c != NULL){
    inv->timeStamp[0] = 0;
    inv->pageNumber[0] = c;
    if(flag == 1)
      inv->ASID[0] = 1;
    else
      inv->ASID[0] = 2;
    printf("%d : %s : %d \n",inv->timeStamp[0],inv->pageNumber[0],inv->ASID[0]);
  }
}

//a function that initialize inverted page table for the simulation
//and initialize every entry with -1 as empty.
pageTable* initializePageTable(){
  pageTable* invertedTable = malloc(sizeof(pageTable));
  invertedTable->timeStamp = malloc(sizeof(int)*4096);
  invertedTable->pageNumber = malloc(4096*(sizeof(char)*8));
  invertedTable->ASID = malloc(sizeof(int)*4096);
  for(int i = 0; i < 4096; i++){
    invertedTable->timeStamp[i] = -1;
    invertedTable->pageNumber[i] = "-1";
    invertedTable->ASID[i] = -1;
  }
  return invertedTable;
}

// A function whose inputs are the trace files names and the allocation 
// strategy 's' for split or 'f' for free for all 
void Simulate(char* fileName1, char* fileName2, char allocation) {
  int state = 1;
  char *allc = getAllocationMode(allocation);
  FILE* f1;
  FILE* f2;
  int process = 1;
  f1 = fopen(fileName1,"r");
  f2 = fopen(fileName2,"r");
  if(f1 == NULL || f2 == NULL){
    puts("invalid input file");
    state = 0;
  }
  if(state){
    pageTable* inv = initializePageTable();  
    processLine(inv,f1,process);
    free(inv);
    fclose(f1);
    fclose(f2);
    printf("%s \n",allc);
  }
}

int main(int argc, char** argv) {
  // Run simulation
  if(argc != 4){
    puts("Incorrect number of input args. Exit");
    return(0);
  }
  char *firstFile = argv[1];
  char *secondFile = argv[2];
  char mode = *argv[3];
  Simulate(firstFile,secondFile,mode);  //run simulation here
  exit(0);
}
