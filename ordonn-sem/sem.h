#include "process.h"

#ifndef __SEM_H
#define __SEM_H

typedef struct sem_s {
	int32_t jetons;
	struct process_list_s *list;
}sem_s;
//TODO
// penser à la destruction du semaphore.

typedef struct mtx_s {
	signed int jeton;
	struct process_list_s *list;
}mtx_s;
//TODO
// penser à la destruction du mutex.

// Element de la liste
typedef struct process_s {
	pcb_s* pcb;
	pcb_s* next;
}process_s;

// Liste des processus d'un semaphore
typedef struct process_list_s {
	process_s* first;
	process_s* last;
}process_list_s;


void sem_init(struct sem_s* sem, unsigned int val);
void sem_up(struct sem_s* sem);
void sem_down(struct sem_s* sem);

mtx_s* mtx_init();
void mtx_lock(struct mtx_s* mutex);
void mtx_unlock(struct mtx_s* mutex);

#endif
