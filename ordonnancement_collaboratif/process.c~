#include "process.h"
#include "allocateMemory.h"

void init_pcb(struct pcb_s* pcb, func_t f, unsigned int stack_size, void* arg)
{
	pcb->pc = f; // pour avoir l'adresse
	pcb->sp = AllocateMemory(stack_size)+(stack_size-1)*4;
	pcb->state = NEW;
	pcb->arg = arg;
}
