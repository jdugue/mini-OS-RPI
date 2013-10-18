#include "sched.h"
#include "dispatcher.h"

void create_process(func_t f, void* args){
	
	//on crée un process et un element de list	
	pcb_s nProcess;

	//on affecte le next du dernier element de la liste au nouvel element
	pList.last->next = &nProcess;

	//on décale le last au nouveau last
	pList.last = &nProcess;

	//on relie le dernier element de la liste au premier
	nProcess.next = pList.first;  

	init_pcb( pcb_s* toInit, func_t f, STACK_SIZE ,void* args );
}

void yield(){
	next_running = current_process->next->pcb;
	ctx_switch();
}

void start_current_process(){
	__asm("mov %0, lr" : "=r"(current_process->pcb->pc));
	__asm("mov %0, sp" : "=r"(current_process->pcb->sp));
}
