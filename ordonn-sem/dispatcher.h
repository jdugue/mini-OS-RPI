#include "process.h"
#include "sched.h"

#ifndef __DISPATCHER_H
#define __DISPATCHER_H

#define STACK_SIZE 1024

/*void ctx_switch(struct pcb_s* pcb);*/

static pcb_s * current_process;

#endif
