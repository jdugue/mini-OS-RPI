#include "sem.h"
#include "dispatcher.h"
#include "sched.h"

extern pcb_s * current_process;

void sem_init(struct sem_s* sem, unsigned int val)
{
	sem = (sem_s*) AllocateMemory(sizeof(sem_s));
	sem->list = (process_list_s*) AllocateMemory(sizeof(process_list_s));
	sem->list->first = 0;
	sem->list->last = 0;
	sem->jetons = val;
}

void sem_down(struct sem_s* sem)
{
	sem->jetons--;
	if ( sem->jetons < 0 )
	{

		process_s* process = (process_s*) AllocateMemory(sizeof(process_s));
		process->pcb = current_process;
		process->next = 0;

		// Ajouter processus à la liste des processus en attente.
		if( sem->list->first == 0 )
		{
			sem->list->first = process;
			sem->list->last = process;
		} 
		else
		{
			sem->list->last->next = process;
			sem->list->last = process;
		}

		// Il n'ya plus de jetons dans le semphore donc le processus est bloqué.
		current_process->state = BLOCKED;
	
		ctx_switch();
	}	
} 

void sem_up(struct sem_s* sem)
{
	sem->jetons++;

	if ( sem->jetons <= 0 ) // à voir cette ligne
	{
		sem->list->first->pcb->state = RUNNING;
		process_s* temp = sem->list->first;
		sem->list->first = sem->list->first->next;

		FreeAllocatedMemory((uint32_t*) temp); //TODO tester cette ligne
	}
	
	//Pas de ctx_switch car le processus continue à s'éxecuter.
}



mtx_s* mtx_init()
{
	mtx_s* mutex = (mtx_s*) AllocateMemory(sizeof(mtx_s));
	mutex->list = (process_list_s*) AllocateMemory(sizeof(process_list_s));
	mutex->list->first = 0;
	mutex->list->last = 0;
	mutex->jeton = 1;
	return mutex;
}

void mtx_lock(struct mtx_s* mutex)
{
	mutex->jeton--;
	if ( mutex->jeton < 0 )
	{

		process_s* process = (process_s*) AllocateMemory(sizeof(process_s));
		process->pcb = current_process;
		process->next = 0;

		// Ajouter processus à la liste des processus en attente.
		if( mutex->list->first == 0 )
		{
			mutex->list->first = process;
			mutex->list->last = process;
		} 
		else
		{
			mutex->list->last->next = process;
			mutex->list->last = process;
		}

		// Il n'ya plus de jetons dans le semphore donc le processus est bloqué.
		current_process->state = BLOCKED;
	
		ctx_switch();
	}	
}

void mtx_unlock(struct mtx_s* mutex)
{
	mutex->jeton++;

	if ( mutex->jeton <= 0 ) // à voir cette ligne
	{
		mutex->list->first->pcb->state = RUNNING;
		process_s* temp = mutex->list->first;
		mutex->list->first = mutex->list->first->next;

		FreeAllocatedMemory((uint32_t*) temp); //TODO tester cette ligne
	}
	
	//Pas de ctx_switch car le processus continue à s'éxecuter.
}
