#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
//#include "simulation.h"
  
//a structure for pagetable
typedef struct pageTable{
  int* timeStamp;
  char** pageNumber;
  int* ASID;
}pageTable;

int MEMSIZE1 = 32;
int MEMSIZE2 = 64;
int MEMSIZE3 = 128;
int FILEONELINE = 1;
int FILETWOLINE = 1;
int PAGEFAULTONE = 0;

pageTable *invTable1;
pageTable *invTable2;
pageTable *invTable3; 


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

int getMemSize(int flag){
  int size = 0;
  switch(flag){
    case 1:
          size = MEMSIZE1;
          break;
    case 2:
          size = MEMSIZE2;
          break;
    case 3:
          size = MEMSIZE3;
          break;
    default:
          puts("error, invalid flag value");
  }
  return size;
}

//function that find empty spot in given table.
//flag is for identifying size of table.
int findEmptySpot(pageTable *table,int flag){
  int state = -1;
  int size = getMemSize(flag);
  for(int i = 0; i < size; i++){
    if(table->timeStamp[i] == -1){
      state = i;
      break;
    }
  }
  return state;
}

//function that check for pagenumber in the page table
//return true if page is found and update time stamp 
//increase number of page fault if not found.
int checkPageNum(pageTable *table,int flag,char *pageNum){
  int state = 0;
  int size = getMemSize(flag);
  for(int i = 0; i < size; i++){
    if(strcoll(table->pageNumber[i],pageNum) == 0){
      table->timeStamp[i] = FILEONELINE;
      state = 1;
      break;
    } else{  
      PAGEFAULTONE++;
    }
  }
  return state;
}

int getLRUspot(pageTable *table,int flag){
  int min = INT_MAX;
  int size = getMemSize(flag);
  int spot = -1;
  for(int i = 0; i < size; i++){
    if(table->timeStamp[i] < min){
      min = table->timeStamp[i];
      spot = i;
    }
  }
  return spot;
}

//function that replace a specific line from the given table with
//new datas
void replaceLineinTable(pageTable *table,int line,char *c,int flag){
  table->timeStamp[line] = FILEONELINE;
  table->pageNumber[line] = c;
  if(flag == 1)
    table->ASID[line] = 1;
  else
    table->ASID[line] = 2; 
}


//function that processes input file line 
//print incoming request page number of file line
int processLine(FILE *f,int flag){
  int eof = 0;
  int emptySpot;
  char address[6];
  char *c = fgets(address,sizeof(address),f);
  if(c != NULL){
    if(!checkPageNum(invTable1,1,c)){
      if((emptySpot = findEmptySpot(invTable1,1)) != -1){
        replaceLineinTable(invTable1,emptySpot,c,1);
      }
      else{
        emptySpot = getLRUspot(invTable1,1);
        replaceLineinTable(invTable1,emptySpot,c,1);
      }
      FILEONELINE++;
      eof = 1;
    }
  }
  return eof;
}

//a function that initialize inverted page table for the simulation
//and initialize every entry with -1 as empty.
pageTable* initializePageTable(int size){
  pageTable* invertedTable = malloc(sizeof(pageTable));
  invertedTable->timeStamp = malloc(sizeof(int)*size);
  invertedTable->pageNumber = malloc(size*(sizeof(char)*6));
  invertedTable->ASID = malloc(sizeof(int)*size);
  for(int i = 0; i < size; i++){
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
  int lineExist = 1;
  f1 = fopen(fileName1,"r");
  f2 = fopen(fileName2,"r");
  if(f1 == NULL || f2 == NULL){
    puts("invalid input file");
    state = 0;
  }
  if(state){
    invTable1 = initializePageTable(MEMSIZE1);  
    while(lineExist){
      lineExist = processLine(f1,process);
    }
    puts("process line done");
    printf("page fault : %d \n",PAGEFAULTONE);
    free(invTable1);
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
