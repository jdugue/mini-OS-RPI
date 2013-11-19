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
    while ( i++ < 2000000);
    i = 0;
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
    while ( i++ < 2000000);
    i = 0;
  }
  fin();
}

void
processus_C()
{
  int i = 0;

  while (1) {
    while ( i++ < 2000000);
    i = 0;
  }
  
}


//------------------------------------------------------------------------
int
start_kernel ( void )
{
  DISABLE_IRQ();
  init_hw();
  malloc_init((void *) HEAP_START);

  create_process(&processus_A, 512, 0);
  create_process(&processus_B, 512, 1);
  create_process(&processus_C, 512, 2);
  
  start_sched();
  
  return 0;
}
