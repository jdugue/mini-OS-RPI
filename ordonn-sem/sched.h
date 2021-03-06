#include "process.h"

#ifndef __SCHED_H
#define __SCHED_H

extern pcb_s * current_process;

typedef struct pcb_list {
	pcb_s* first;
	pcb_s* last;
}pcb_list;

static pcb_list pList;

void create_process(func_t f, void* args);
void __attribute__((naked)) ctx_switch();
void start_current_process();

#endif
