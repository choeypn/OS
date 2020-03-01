/*
 *  File: Simulation.c
 *  Name: Nat Choeypant
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>

//A structure representing a simplified Process Control Block. It includes
typedef struct PCB
{
  int name;
  int timeLeft;
  int waitTime;
  int burstTime;
  int quantumAExceededCount;
  // an integer indicating the number of times a process has exceeded its quantum A. Used to decide whether it should be demoted or not.
  bool demoted;
  // bool to indicate whetehr a process has been demoted or not. This is checked to decide where it should be requeued.
  struct PCB* nextProcess;
} PCB;

#include "Simulation.h"

PCB* currentCPUProcess; // A pointer to current process occupying the CPU
PCB* QueueAHead;
PCB* QueueBHead;
PCB* QueueATail;
PCB* QueueBTail;


int quantumA = 5;
int quantumB = 40;
int dispatchRatio;
int dispatchACount = 0;            // current dispatch count from Queue A
int demotionThreshold;
int processesCompleted = 0;
int acceptedJobs = 0;
int executionTime = 0;
int idleTime = 0;             // the time the CPU spent as idle (no current process)
int totalWaitTime = 0;
int maxWaitTime = 0;
int minWaitTime = INT_MAX;
int pNum = 1;


// Adds a PCB to the tail of QueueA
void addToAQueue(PCB* toAdd)
{
  if(QueueAHead == NULL){
    QueueAHead = toAdd;
    QueueATail = toAdd;
  }else{
    PCB *pointer = QueueAHead;
    while(pointer->nextProcess != NULL)
      pointer = pointer->nextProcess;
    pointer->nextProcess = toAdd;
    QueueATail = pointer->nextProcess;
  }
}

// Adds a PCB to the tail of QueueB.
void addToBQueue(PCB* toAdd)
{
  if(QueueBHead == NULL){
    QueueBHead = toAdd;
    QueueBTail = toAdd;
  }else{
    PCB *pointer = QueueBHead;
    while(pointer->nextProcess != NULL)
      pointer = pointer->nextProcess;
    pointer->nextProcess = toAdd;
    QueueBTail = pointer->nextProcess;
  }
}


// a function that removes a process from QueueA. If QueueA is empty,
// it retrieves a process from QueueB or null if QueueB is empty as well.
PCB* removeFromQueueAFirst()
{
  PCB *output = NULL;
  if(QueueAHead == NULL)
    output = QueueBHead;
  else{
    output = QueueAHead;
    QueueAHead = QueueAHead->nextProcess;
  }
  return output;
}

// a function that removes a process from QueueB. If QueueB is empty,
// it retrieves a process from QueueA or null if QueueA is empty as well.
PCB* removeFromQueueBFirst()
{
  PCB *output = NULL;
  if(QueueBHead == NULL)
    output = QueueAHead;
  else{
    output = QueueBHead;
    QueueBHead = QueueBHead->nextProcess;
  }
  return output;
}


// Retrieves a process statistics and terminates it by freeing its memory
void terminateProcess()
{
  free(currentCPUProcess);
}

// prints final result for current run.
void printFinalStatistics()
{
// to be implemented
}

// reads next job from the job file and added to the tail queueA.
// returns false if EOF is reached and true otherwise
bool queueJobFromFile(FILE *file)
{
  bool eof = false;
  char line[10];
  if(fgets(line,sizeof(line),file) != NULL){
    if(line[0] != 'i'){   //check if new process is coming in
      PCB *p = malloc(sizeof(PCB));
      p->name = pNum;
      p->timeLeft = atoi(line);
      p->waitTime = 0;
      p->burstTime = 0;
      p->quantumAExceededCount = 0;
      p->demoted = false;
      p->nextProcess = NULL;
      printf("process number: %d timeleft: %d \n",p->name,p->timeLeft);
      addToAQueue(p);
      pNum++;
    }
    eof = true;
  }
  return eof;
}

// Retrieves a process from QueueA or QueueB depending on the dispatch ratio and
//current dispatch count. Returns a pointer to the PCB curretnly occupying the CPU
PCB* dispatchProcessFromQueues()
{
  PCB* process;
  if(dispatchACount == dispatchRatio){
    process = removeFromQueueBFirst();
    dispatchACount = 0;
  }else{
    process = removeFromQueueAFirst();
    dispatchACount++;
  }
  return process;
}

// perform a "tick" by updating wait times, execution time, process time left ...
// the updated variables can be global or specific for each PCB
void updateAllTimes ()
{

}

bool checkQuantum(){
  bool state = false;
  bool check = false;
  if(currentCPUProcess->quantumAExceededCount < demotionThreshold)
    check = true;
  if(check){
    if(currentCPUProcess->burstTime == quantumA){
      state = true;
      currentCPUProcess->burstTime = 0;
    }
  }else{
    if(currentCPUProcess->burstTime == quantumB){
      state = true;
      currentCPUProcess->burstTime = 0;
    }
  }
  return state;
}

// At the start of an execution cycle, this fucntion is called to decide whether
// current process will keep using the CPU for next cycle or it will exit and a
// process will be dispatched. The process would exit if it finishes it execution
// time or when the current quantum expires.
void continueOrExitAndDispatch()
{
  if(currentCPUProcess == NULL)
    currentCPUProcess = dispatchProcessFromQueues();
  else if(checkQuantum() || currentCPUProcess->timeLeft == 0){
    exitCPU();
    currentCPUProcess = dispatchProcessFromQueues();
  }
}

// when this function is called, the current function occupying the CPU will exit
// and would either terminate if its execution time is done or get requeued (with possible demotion)
void exitCPU()
{
  if(currentCPUProcess->timeLeft == 0)
    terminateProcess();
  else{
    if(currentCPUProcess->demoted)
      addToBQueue(currentCPUProcess);
    else{
      updateDemotionCountAndFlag();
      addToAQueue(currentCPUProcess);
    }
  }
}


// called when a non-demoted process exits due to exceeding its burst time. Processes
// that are already demoted do not need to be checked/updated as demotuion is irreversable.
void updateDemotionCountAndFlag()
{
  currentCPUProcess->quantumAExceededCount++;
  if(currentCPUProcess->quantumAExceededCount == demotionThreshold-1)
    currentCPUProcess->demoted = true;
}


 // During each 'tick' calls should follow this order
 // continueOrExitAndDispatch > dispatchProcessFromQueues (called inside continueOrExitAndDispatch() if needed) > queueJobFromFile > updateAllTimes
 // Other functions or code could and should be used between or inside
 // these calls.
void Simulate(char *filename,int demotionThreshold, int dispatchRatio) {
  printf("Demotion threshold value received : %d\n", demotionThreshold);
  printf("Dispatch ratio value received : %d\n", dispatchRatio);
  FILE *f;
  f = fopen(filename,"r");
  bool processFile = true;
  while(processFile){
    continueOrExitAndDispatch();
    processFile = queueJobFromFile(f);
    //updateAllTimes();
  }
  fclose(f);
  puts("read queue from file done");
}

int main(int argc, char** argv)
{
  // parse inputs (from Lab 3)
  if(argc != 4){
    puts("Incorrect number of input arguments. Exit Main");
    return(0);
  }
  char *filename = argv[1];
  demotionThreshold = atoi(argv[2]);
  dispatchRatio = atoi(argv[3]);
  Simulate(filename,demotionThreshold,dispatchRatio);   //run simulation
  exit(0);
}
