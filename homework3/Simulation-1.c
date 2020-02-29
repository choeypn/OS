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
  int timeLeft;
  int waitTime;
  int quantumAExceededCount;    
  // an integer indicating the number of times a process has exceeded its quantum A. Used to decide whether it should be demoted or not.
  bool demoted;                 
  // bool to indicate whetehr a process has been demoted or not. This is checked to decide where it should be requeued.
  struct PCB* nextProcess;
} PCB;

PCB* currentCPUProcess; // A pointer to current process occupying the CPU
PCB* QueueAHead;
PCB* QueueBHead;
PCB* QueueATail;
PCB* QueueBTail;


int quantumA;
int quantumB;
int dispatchRatio;
int dispatchACount;            // current dispatch count from Queue A
int demotionThreshold;
int processesCompleted = 0;
int acceptedJobs = 0;
int executionTime = 0;
int idleTime = 0;             // the time the CPU spent as idle (no current process)
int totalWaitTime = 0;
int maxWaitTime = 0;
int minWaitTime = INT_MAX;



// Adds a PCB to the tail of QueueA
void addToAQueue(PCB* toAdd) 
{
// to be implemented
}

// Adds a PCB to the tail of QueueB.
void addToBQueue(PCB* toAdd) 
{
// to be implemented
}


// a function that removes a process from QueueA. If QueueA is empty, 
// it retrieves a process from QueueB or null if QueueB is empty as well.
PCB* removeFromQueueAFirst() 
{
// to be implemented
}

// a function that removes a process from QueueB. If QueueB is empty, 
// it retrieves a process from QueueA or null if QueueA is empty as well.
PCB* removeFromQueueBFirst() 
{
// to be implemented
}


// Retrieves a process statistics and terminates it by freeing its memory
void terminateProcess() 
{
// to be implemented
}

// prints final result for current run.
void printFinalStatistics() 
{
// to be implemented
}

// reads next job from the job file and added to the tail queueA.
// returns false if EOF is reached and true otherwise
bool queueJobFromFile()
{
  // to be implemented
}

// Retrieves a process from QueueA or QueueB depending on the dispatch ratio and 
//current dispatch count. Returns a pointer to the PCB curretnly occupying the CPU
PCB* dispatchProcessFromeQueues()
{
  // to be implemented
}

// perform a "tick" by updating wait times, execution time, process time left ...
// the updated variables can be global or specific for each PCB
void updateAllTimes ()
{
  // to be implemented
}

// At the start of an execution cycle, this fucntion is called to decide whether
// current process will keep using the CPU for next cycle or it will exit and a
// process will be dispatched. The process would exit if it finishes it execution
// time or when the current quantum expires.
void continueOrExitAndDispatch()
{
  // to be implemented
}

// when this function is called, the current function occupying the CPU will exit
// and would either terminate if its execution time is done or get requeued (with possible demotion)
void exitCPU() 
{
  //to be implemented
}


// called when a non-demoted process exits due to exceeding its burst time. Processes
// that are already demoted do not need to be checked/updated as demotuion is irreversable.
void updateDemotionCountAndFlag()
{
  //to be implemented
}

void Simulate(char *filename,int demotionThreshold, int dispatchRatio) {
  // A function whose input is the demotion counter for a queue A and the
  // dispatch ratio between the two queueus
  printf("Input file name : %s\n", filename);
  printf("Demotion threshold value received : %d\n", demotionThreshold);
  printf("Dispatch ratio value received : %d\n", dispatchRatio); 
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
 // Run simulation
  Simulate(filename,demotionThreshold,dispatchRatio);
  exit(0);
 // During each 'tick' calls should follow this order
 // contintueOrExitAndDispatch > dispatchProcessFromQueues (called inside continueOrExitAndDispatch() if needed) > queueJobFromFile > updateAllTimes
 // Other functions or code could and should be used between or inside 
 // these calls.
}
