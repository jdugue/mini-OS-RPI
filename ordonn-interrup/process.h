#include <stdint.h>

#ifndef __PROCESS_H
#define __PROCESS_H

typedef void (*func_t) (void*);

typedef enum {READY, RUNNING, BLOCKED, NEW, TERMINATED} pState;

typedef struct pcb_s {
	uint32_t * sp ; 
	func_t pc; 
	pState state;
	void * arg;
	struct pcb_s* next;
	// uint32_t registres[13];  // dans la pile
}pcb_s;

void init_pcb(pcb_s* pcb, func_t f, unsigned int stack_size, void* arg);

#endif
