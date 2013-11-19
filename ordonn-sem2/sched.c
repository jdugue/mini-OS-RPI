#include "sched.h"
#include "dispatcher.h"
#include "hw.h"

#define STACK_SIZE 1024

pcb_s * current_process;

void create_process(func_t f, void* args, int period, int calcul){
	
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

	init_pcb( nProcess, f, STACK_SIZE , args, period, calcul );
}

void __attribute__((naked)) ctx_switch() {
	
	DISABLE_IRQ();
	
	__asm("sub lr, lr, #4");
	__asm("srsdb sp!, #0x13");
	__asm("cps #0x13");

	//Save r0-r12
	//A vérifier : Pas de destination car par defaut push sur la pile
	__asm volatile("push {r0-r12,lr}"); 
	__asm("mov %0, sp": "=r"(current_process->sp));

	
	// Choisi le prochain processus a executer
	//schedule();
	select_next();
	
	if(current_process->state == NEW)
	{
		
		__asm("mov sp, %0" : : "r"(current_process->sp));
		current_process->state=RUNNING;
		
		// Rearme du timer
		set_next_tick_and_enable_timer_irq();
		ENABLE_IRQ();
		
		current_process->pc(current_process->arg);
		current_process->state=TERMINATED;
		ctx_switch();
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

		//TODO
		//Se rappeler de liberer la memoire du pcb.
	}
	
	// Rearme du timer
	set_next_tick_and_enable_timer_irq();
	ENABLE_IRQ();
	
	__asm("rfefd sp!");
}


void start_scheduler()
{
	//current_process = (pcb_s**) AllocateMemory(sizeof(pcb_s*));
	pcb_s* tempProcess = (pcb_s*) AllocateMemory(sizeof(pcb_s));
	init_pcb(tempProcess, 0, STACK_SIZE, 0, 1000, 1000);
	tempProcess->state  = TERMINATED;
	tempProcess->next = pList.first;
	//pList.last->next = pList.first;
	current_process = tempProcess;
	//current_process = pList.first;
	
	DISABLE_IRQ();
	init_hw();
	set_next_tick_and_enable_timer_irq();
	ENABLE_IRQ();
}

void schedule() {
	//TODO
	// Confirmation prof
	do 
	{
		current_process = current_process->next;
	}
	while ( current_process->state == BLOCKED );

}

void
start_sched()
{
	/*
  current_process = &idle;
  idle.next = ready_queue;
  idle.period = 1000;
  idle.calcul = 1000;
  idle.period_remaining = 0;
  idle.calcul_remaining = 0;

  ENABLE_IRQ();

  while(1) {
    yield();
  }
  */
}

void select_next(struct pcb_s* pcbs)
{
	//current_process = pcbs;

	struct pcb_s* pcb;
	struct pcb_s* pcb_selected;

	pcb_selected = 0;
	pcb = pList.first;

	do
	{
		// Gestion des remainings
		if ( !pcb->period_remaining )
		{
			pcb->period_remaining = pcb->period;
			pcb->calcul_remaining = pcb->calcul;
		}
		
		pcb->period_remaining--;
		pcb = pcb->next; 
	}while(pcb != pList.first);
	
	pcb = pList.first;
    do
    {			
		// Selection du process
		if ( pcb->state == RUNNING || pcb->state == NEW)
		{
			if ( pcb->calcul_remaining )
			{
				if (!pcb_selected) 
				{
					pcb_selected = pcb;
				}
				else if( pcb->period < pcb_selected->period )
				{
					pcb_selected = pcb;
				}
				else if ( pcb->period == pcb_selected->period && 
					pcb->period_remaining < pcb_selected->period_remaining )
				{
					pcb_selected = pcb;
				}
			}
		}	
		pcb = pcb->next; 
	}while(pcb != pList.first);
	

	// ici selected = &idle
	// ici passe plusiers fois dans la boucle, au moins 2
	
	current_process = pcb_selected;
	current_process->calcul_remaining--; 
}

