//header file for Simulation.c
//Nat Choeypant

void addToAQueue(PCB* toAdd);
void addToBQueue(PCB* toAdd);

PCB* removeFromQueueAFirst();
PCB* removeFromQueueBFirst();

void terminateProcess();
void printFinalStatistics();

bool queueJobFromFile(FILE *file);

PCB* dispatchProcessFromQueues();

void updateAllTimes();

bool checkQuantum();

void continueOrExitAndDispatch();
void exitCPU();
void updateDemotionCountAndFlag();
void Simulate(char *filename,int demotionThreshold,int dispatchRatio);

