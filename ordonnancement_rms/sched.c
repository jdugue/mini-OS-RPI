#include "sched.h"
#include "malloc.h"
#include "hw.h"
#include "dispatcher.h"

struct pcb_s idle;
struct pcb_s* ready_queue = (struct pcb_s *)0;
struct pcb_s* current_process = (struct pcb_s *) NULL;

extern void processus_A();

#define PRINT(MSG) ;
#define EXIT(CODE) ;

void
start_current_process()
{
  current_process->state = READY;
  current_process->entry_point( current_process->args );

  /* The process is terminated */
  current_process->state = TERMINATED;
  yield();
}

int
init_process(struct pcb_s *pcb, int stack_size, func_t* f, int period, int calcul)
{	
  /* Function and args */
  pcb->entry_point = f;

  /* Stack allocation */
  pcb->size=stack_size;
  pcb->stack_base = malloc_alloc(stack_size);
  if(!pcb->stack_base)
    return 0;

  /* State and context */
  pcb->state = NEW;
  pcb->sp = ((uint32_t*) (pcb->stack_base + stack_size)) - 1;
  
  /* SET RMS attributes */
  pcb->calcul = calcul;
  pcb->period = period;
  pcb->calcul_remaining = 0;
  pcb->period_remaining = 0;
  
  return 1;
}

int
create_process(func_t* f, unsigned size, int period, int calcul)
{
  struct pcb_s *pcb;
  pcb = (struct pcb_s*) malloc_alloc(sizeof(struct pcb_s));

  if(!pcb)
    return 0;

  if (! ready_queue) {/* First process */
    ready_queue = pcb;
  } else {
    pcb->next = ready_queue->next;
  }
  
  ready_queue->next = pcb;
  return init_process(pcb,size,f,period,calcul);
}


void
schedule()
{
  struct pcb_s* pcb;
  struct pcb_s* pcb_init;

  pcb_init = current_process;
  pcb = current_process;

  /* Start by eliminating all zombies (rhaaaaa !) */
  while (pcb->next->state == TERMINATED) {    
    /* If no process to be executed -> note that and leave loop */
    if (pcb->next == pcb) {
      pcb = NULL;
      break;

    } else {
      /* Particular case of the head */
      if (pcb->next == ready_queue)
	ready_queue = pcb;    
      
      /* Remove pcb from the list (FIXME : could be done after the loop) */
      pcb->next = pcb->next->next;

      /* Free memory */
      malloc_free((char*) pcb->next->stack_base);
      malloc_free((char*) pcb->next);

      /* Get next process */
      pcb = pcb->next;
    }
  }

  if (pcb != NULL) {
    /* On parcours la liste jusqu'à trouver un processus non bloqué */
    pcb = pcb->next;    
    while(pcb->state == WAITING && pcb != pcb_init)
      pcb = pcb->next;

    /* Si tous les processus sont bloqués -> on le note */
    if(pcb->state == WAITING)
      pcb = NULL;
  }

  if(pcb == NULL) {   /* Si pas de processus à élire -> idle */
    ready_queue = NULL;
    current_process = &idle;
  } else {            
	  /* Sinon -> le processus élu est trouve par l'algorithme RMS*/
      select_next(pcb);
	  //current_process = pcb;
  }
}

void
yield()
{
  ctx_switch();
}

void
start_sched()
{
  current_process = &idle;
  idle.next = ready_queue;
  idle.period = 1000;
  idle.calcul = 1000;
  idle.period_remaining = 0;
  idle.calcul_remaining = 0;

  //ENABLE_IRQ();

  while(1) {
    yield();
  }
}

void select_next(struct pcb_s* pcbs)
{
	struct pcb_s* pcb;
	struct pcb_s* pcb_init;
	struct pcb_s* pcb_selected;

	pcb_init  = ready_queue;
	pcb_selected = &idle;
	pcb = pcb_init;

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
	}while(pcb != pcb_init);
	
    do
    {			
		// Selection du process
		if ( pcb->state == READY || pcb->state == NEW)
		{
			if ( pcb->calcul_remaining )
			{
				if( pcb->period < pcb_selected->period )
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
	}while(pcb != pcb_init);
	

	// ici selected = &idle
	// ici passe plusiers fois dans la boucle, au moins 2
	
	current_process = pcb_selected;
	//current_process = current_process->next;
	current_process->calcul_remaining--; 
}
