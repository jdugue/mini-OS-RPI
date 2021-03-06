#include "sched.h"
#include "dispatcher.h"

void create_process(func_t f, void* args){
	
	//on crée un process et un element de list	
	pcb_s* nProcess = (pcb_s*) AllocateMemory(sizeof(pcb_s));

	//on affecte le next du dernier element de la liste au nouvel element
	if (pList.last == 0)
	{
		pList.last = nProcess;
	}	
	pList.last->next = nProcess;

	//on décale le last au nouveau last
	pList.last = nProcess;

	//on relie le dernier element de la liste au premier
	if (pList.first == 0)
	{
		pList.first = nProcess;
	}	

	nProcess->next = pList.first;  

	init_pcb( nProcess, f, STACK_SIZE , args );
}

void yield(){

	//Save r0-r12
	__asm volatile("push {r0-r12,lr}"); //A vérifier : Pas de destination car par defaut push sur la pile

	__asm("mov %0, sp": "=r"(current_process->sp));

	
	current_process = current_process->next;
	
	if(current_process->state == NEW)
	{
		__asm("mov sp, %0" : : "r"(current_process->sp));
		current_process->state=RUNNING;
		current_process->pc(current_process->arg);
		current_process->state=TERMINATED;
		yield();
		//__asm("mov lr, %0" : : "r"(current_process->pc));
	}
	else if (current_process->state == RUNNING)
	{
		__asm("mov sp, %0" : : "r"(current_process->sp));

		//Restituer les registres
		__asm volatile("pop {r0-r12,lr}"); 

	}
	

	
	if (current_process->next->state == TERMINATED)
	{
		if (pList.last == current_process->next)
		{
			pList.last = current_process->next->next;
			current_process->next = pList.last; 
		}
		else if (pList.first == current_process->next)
		{
			pList.first = current_process->next->next;
			pList.last->next = pList.first;
		}
		else
		{
			current_process->next = current_process->next->next;
		}
	}
		
}

void killme()
{
	current_process->state = TERMINATED;
	yield();
}

void start_scheduler()
{
	pcb_s tempProcess;
	init_pcb(&tempProcess,0, STACK_SIZE , 0);
	tempProcess.state = TERMINATED;
	tempProcess.next = pList.first;
	current_process = &tempProcess; 
	yield();
}

