#include "hw.h"
#include "sched.h"
#include "malloc.h"

extern void play_music();

void fin()
{
	int i=0;
}
void
processus_A()
{
  int i = 0;
  
  while (1) 
  {
    led_off();
  }
  fin();
}

void
processus_B()
{
  int i = 0;

  while (1) 
  {
    led_on();
  }
  fin();
}

void
processus_C()
{
  int i = 0;

  while (1) {
    i++;
  }
  
}


//------------------------------------------------------------------------
int
start_kernel ( void )
{
  DISABLE_IRQ();
  init_hw();
  malloc_init((void *) HEAP_START);

  create_process(&processus_A, 512, 10,  1, 10,  0);
  create_process(&processus_B, 512, 10,  1,  5,  0);
  create_process(&processus_C, 512, 20, 18, 20, 18);
  
  start_sched();
  
  return 0;
}
