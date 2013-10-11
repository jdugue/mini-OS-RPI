#include <stdint.h>

typedef void (*func_t) ( void);

typedef enum {READY = 1, RUNNING = 2, BLOCKED = 3, NEW = 4, TERMINATED = 5} pState;

typedef struct pcb_s {
	uint32_t * sp ; //equivalent a uint32_t ?
	uint32_t * pc; //idem
	pState state;
	void * arg;
	// uint32_t registres[13];  // dans la pile
}pcb_s;

void init_pcb(pcb_s* pcb, func_t f, unsigned int stack_size, void* arg);
