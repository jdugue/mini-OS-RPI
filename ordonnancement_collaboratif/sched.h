#include "process.h"

#ifndef __SCHED_H
#define __SCHED_H

static pcb_s* next_running;

typedef struct pcb_list {
	pcb_s* first;
	pcb_s* last;
}pcb_list;

static pcb_list pList;

void create_process(func_t f, void* args);
void yield();
void killme();
void start_current_process();

#endif
