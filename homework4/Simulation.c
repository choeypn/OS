#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
//#include "simulation.h"
 
int MEMSIZE1 = 32;
int MEMSIZE2 = 64;
int MEMSIZE3 = 128;
int FILEONELINE = 0;
int FILETWOLINE = 0;
int PAGEFAULTONE = 0; 
int PAGEFAULTTWO = 0;
int PAGEFAULTTHREE = 0;

char ALLC;

typedef struct pageTable{
  int* timeStamp;
  char** pageNumber;
  int* ASID;
}pageTable;

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

//get size from flag
// 32, 64, 126
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

//increment number of page fault from flag
// 32, 64, 128
void incrementPageFault(int flag){
  switch(flag){
    case 1:
          PAGEFAULTONE++;
          break;
    case 2:
          PAGEFAULTTWO++;
          break;
    case 3:
          PAGEFAULTTHREE++;
          break;
    default:
          puts("error, invalid flag value");
  }
}

//function that find empty spot in given table.
//flag is for identifying size of table.
int findEmptySpot(pageTable *table,int flag,int process){
  int spot = -1;
  int size = getMemSize(flag);
  int start = 0;
  int end = size;
  if(ALLC == 's'){
    if(process == 0) 
      end = size/2;
    else
      start = size/2;  
  }
  for(int i = start; i < end; i++){
    if(table->timeStamp[i] == -1){
      spot = i;
      break;
    }
  }
  return spot;
}

//handle check pagenumber for free for all
//return true if page found
int handleFreeForAll(pageTable* table,int size,char *pageNum,int process){
  int state = 0;
  for(int i = 0; i < size; i++){
    if(strcoll(table->pageNumber[i],pageNum) == 0){
      if(table->ASID[i] == process){
        if(process == 0)
          table->timeStamp[i] = FILEONELINE;
        else
          table->timeStamp[i] = FILETWOLINE;
        state = 1;
        break;
      }
    }  
  }
  return state;
}

//handle check pagenumber for split
//check for existing pagenumber in process frames from given process
int handleSplit(pageTable* table,int size,char *pageNum,int process){
  int state = 0;
  int start = 0;
  int end = size;
  if(process == 0)
    end = size/2;
  else
    start = size/2;
  for(int i = start; i < end; i++){
    if(strcoll(table->pageNumber[i],pageNum) == 0){
      if(process == 0)
        table->timeStamp[i] = FILEONELINE;
      else
        table->timeStamp[i] = FILETWOLINE;
      state = 1;
      break;
    }
  }
  return state;
}

//function that check for pagenumber in the page table
//return true if page is found and update time stamp 
//increase number of page fault if not found.
int checkPageNum(pageTable *table,int flag,char *pageNum,int process){
  int state = 0;
  int size = getMemSize(flag);
  if(ALLC != 's')
    state = handleFreeForAll(table,size,pageNum,process);
  else
    state = handleSplit(table,size,pageNum,process);
  return state;
}

//function that find the least recently used spot in the table.
//return line # of LRU
int findLRUSpot(pageTable *table,int flag,int process){
  int min = INT_MAX;
  int size = getMemSize(flag);
  int spot = -1;
  int start = 0;
  int end = size;
  if(ALLC == 's'){
    if(process == 0)
      end = size/2;
    else
      start = size/2;
  }
  for(int i = start; i < end; i++){
    if(table->timeStamp[i] < min){
      min = table->timeStamp[i];
      spot = i;
    }
  }
  return spot;
}

//function that replace a specific line from the given table with
//new datas
void replaceLineinTable(pageTable *table,int line,char *c,int process){
  table->timeStamp[line] = FILEONELINE;
  table->pageNumber[line] = strdup(c);
  if(process == 0)
    table->ASID[line] = 0;
  else
    table->ASID[line] = 1; 
}

//function that check if page number exists in the table
//update time stamp if exist, handle finding spot if not.
void processMemory(pageTable *table,int flag,char *in,int process){
  int emptySpot = -1;
  if(!checkPageNum(table,flag,in,process)){
    incrementPageFault(flag);
    if((emptySpot = findEmptySpot(table,flag,process)) != -1){
      replaceLineinTable(table,emptySpot,in,process);
    }
    else if((emptySpot = findLRUSpot(table,flag,process)) != -1){
      replaceLineinTable(table,emptySpot,in,process);
    }
    else{
      puts("error. something wrong");
    }
  }
}


//function that processes input file line 
//print incoming request page number of file line
int processLine(FILE *f1,FILE *f2,int process){
  int eof = 0;
  char address[12];
  char *in;
  if(process == 0)
    in = fgets(address,sizeof(address),f1);
  else
    in = fgets(address,sizeof(address),f2);
  if(in != NULL){
    in[5] = '\0';
    processMemory(invTable1,1,in,process);
    if(process == 0)
      FILEONELINE++;
    else
      FILETWOLINE++;   
   eof = 1; 
  }
  return eof;
}

//a function that initialize inverted page table for the simulation
//and initialize every entry with -1 as empty.
pageTable* initializePageTable(int size){
  pageTable* invertedTable = malloc(sizeof(pageTable));
  invertedTable->timeStamp = malloc(sizeof(int)*size);
  invertedTable->pageNumber = malloc(sizeof(char*)*size);
  for(int i = 0; i < size; i++)
    invertedTable->pageNumber[i] = malloc(sizeof(char)*6);
  invertedTable->ASID = malloc(sizeof(int)*size);
  for(int i = 0; i < size; i++){
    invertedTable->timeStamp[i] = -1;
    invertedTable->pageNumber[i] = "-1";
    invertedTable->ASID[i] = -1;
  }
  return invertedTable;
}

//print final stat once processing completed.
void printFinalStat(){
  printf("Allocation: %s \n",getAllocationMode(ALLC));
  printf("32 frames page fault: %d \n",PAGEFAULTONE);
}

// A function whose inputs are the trace files names and the allocation 
// strategy 's' for split or 'f' for free for all 
void Simulate(char* fileName1, char* fileName2) {
  int state = 1;
  FILE* f1;
  FILE* f2;
  int process = 0;
  int lineExist = 1;
  int track = 1;
  f1 = fopen(fileName1,"r");
  f2 = fopen(fileName2,"r");
  if(f1 == NULL || f2 == NULL){
    puts("invalid input file");
    state = 0;
  }
  if(state){
    invTable1 = initializePageTable(MEMSIZE1);  
    while(lineExist){
      lineExist = processLine(f1,f2,process);
      if(track % 20 == 0)
        process = (process+1) % 2;
      track++;
    }
    puts("process line done");
    printFinalStat();
    free(invTable1);
    fclose(f1);
    fclose(f2);
  }
}

int main(int argc, char** argv) {
  if(argc != 4){
    puts("Incorrect number of input args. Exit");
    return(0);
  }
  char *firstFile = argv[1];
  char *secondFile = argv[2];
  ALLC = *argv[3];
  Simulate(firstFile,secondFile);  //run simulation here
  exit(0);
}
