#ifndef _PSTAT_H_
#define _PSTAT_H_

#include "param.h"

struct pstat {
  int num_processes;
  int pid[NPROC];     // the PID of each process 
  int tickets[NPROC]; // the number of tickets this process has
  int ticks[NPROC];   // the number of ticks each process has accumulated 
};

#endif // _PSTAT_H_
