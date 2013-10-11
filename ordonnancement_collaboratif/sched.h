#include "process.h"
static pcb_s* next_running;

typedef struct pcb_element {
	pcb_s* pcb;
	pcb_s* next;
}pcb_element;


typedef struct pcb_list {
	pcb_element* first;
	pcb_element* last;
}pcb_list;

static pcb_list pList;

void create_process(func_t f, void* args);
void yield();
void start_current_process();
