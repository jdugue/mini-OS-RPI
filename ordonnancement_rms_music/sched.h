#ifndef SCHED_H
#define SCHED_H

#include <stdint.h>
#define NULL 0x0

extern struct pcb_s* current_process;

typedef void (func_t)( void );

enum pcb_state_e {READY, NEW, TERMINATED, WAITING};

struct pcb_s {
  /* Pointer to stack */
  uint32_t* sp;
    
  /* function and args */
  func_t* entry_point;
  void* args;
  
  unsigned int size;
  char* stack_base; //sp
  enum pcb_state_e state;

  struct pcb_s *next;
  
  //RMS
  int period;
  int calcul;
  
  int period_remaining; // period remaining
  int calcul_remaining;  // calcul remaining
  
};

int create_process(func_t* f, unsigned size, int period, int calcul);
void yield();
void start_sched();
void schedule();
void start_current_process();
void select_next(struct pcb_s* pcbs);


#endif
