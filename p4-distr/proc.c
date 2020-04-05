// ONLY MODIFY CODE BELOW THE NEXT OCCURENCE OF THE FOLLOWING LINE !
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "types.h"
#include "defs.h"
#include <stdio.h>
#include "proc.h"

#define NCPU 1

struct cpu cpus[NCPU];
int ncpu;

void printstate(enum procstate pstate){ // DO NOT MODIFY
  switch(pstate) {
    case UNUSED   : printf("UNUSED");   break;
    case EMBRYO   : printf("EMBRYO");   break;
    case SLEEPING : printf("SLEEPING"); break;
    case RUNNABLE : printf("RUNNABLE"); break;
    case RUNNING  : printf("RUNNING");  break;
    case ZOMBIE   : printf("ZOMBIE");   break;
    default       : printf("????????");
  }
}

// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run
//  - swtch to start running that process
//  - eventually that process transfers control
//      via swtch back to the scheduler.

// local to scheduler in xv6, but here we need them to persist outside,
// because while xv6 runs scheduler as a "coroutine" via swtch,
// here swtch is just a regular procedure call.
int pix;
struct proc *p;
struct cpu *c = cpus;

// +++++++ ONLY MODIFY BELOW THIS LINE ++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void
scheduler(void)
{ int runnableFound; // DO NOT MODIFY/DELETE
  c->proc = 0;

  runnableFound = 1 ; // force one pass over ptable

  //make array of all priorities, then sort
  //----------initialization------------
  
  int prios[NPROC] = {0};


  int pindex = 0; 
  for(int i =0; i<NPROC; i++){

      p = &ptable.proc[i];
      int prio = p->prio;
      int found = 0;
      for(int j =0; j<NPROC; j++){
        if(prios[j] == prio){
          found = 1;
          break;
        }
      }
    if(found == 0)
      prios[pindex++]= prio;
    
  }

  int j;
  for (int i = 1; i < NPROC; i++) { 
    int element = prios[i]; j = i - 1; 
      while (j >= 0 && prios[j] > element) { 
          prios[j + 1] = prios[j]; 
          j = j - 1; 
      } 
      prios[j + 1] = element; 
  } 
  int minp = prios[0], priox = 0;
  //-----------------end init-------------------

  
  int still_running = 1;//force one pass over the array

  while(still_running){
    still_running = 0;
    int NPROCsa = 0; 
    struct proc *PROCsa[NPROC] = {NULL};
    for(int k = 0, i = 0; k<NPROC; k++){
      p = &ptable.proc[k];
      if(p->prio == minp) {
          NPROCsa++;
          PROCsa[i++] = p;
      }
    }
    runnableFound = 1;
    while(runnableFound){ // DO NOT MODIFY
      runnableFound = 0; // DO NOT MODIFY
      for(pix = 0; pix < NPROCsa; pix++){
        p = PROCsa[pix];
        if(p == NULL) continue;
        if(p->state != RUNNABLE)
          continue; //get next p
        runnableFound = 1; // DO NOT MODIFY/DELETE/BYPASS
        c->proc = p;
        p->state = RUNNING;
        swtch(p); //run the process
        c->proc = 0;
        
      }
    }

    for(pix = 0; pix< NPROC; pix++){
      p = &ptable.proc[pix];
      if(p->state != RUNNABLE)
        continue;

      still_running = 1;
      break;
    }
    minp = prios[priox++];
  }
  printf("No RUNNABLE process!\n");
}

